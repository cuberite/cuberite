-- UpgradePaletteCheck.lua

--[[ Implements a check that lists all entries in the UpgradeBlockTypePalette
that are not present in the 1.13 protocol palette.

The palette files are expected to be in folders relative to this script like
they are in the git repo:
../../Server/Protocol/UpgradeBlockTypePalette.txt
../../Server/Protocol/1.13/base.btp.txt
--]]





--- Partially loads a BlockTypePalette file
-- Parses the specified file into headers (returned as a key-value table) and data (returned as array of strings)
-- Returns a table, {headers = {}, data = {}}
local function loadPalSplitHeader(aFileName)
	local f = assert(io.open(aFileName, "r"))
	local isFileIdentLine = true
	local isInHeaders = false
	local headers = {}
	local data = {}
	local idx = 1
	for ln in f:lines() do
		if (isFileIdentLine) then
			assert(not(isInHeaders))
			assert((ln == "BlockTypePalette") or (ln == "UpgradeBlockTypePalette"))
			isFileIdentLine = false
			isInHeaders = true
		elseif (ln == "") then
			assert(not(isFileIdentLine))
			assert(isInHeaders)  -- data must not contain an empty line
			isInHeaders = false
		elseif (isInHeaders) then
			assert(not(isFileIdentLine))
			local key, val = ln:match("(.-)\t(.*)")
			assert(key)
			assert(val)
			headers[key] = val
		else
			data[idx] = ln
			idx = idx + 1
		end
	end
	return { headers = headers, data = data}
end





--- Normalizes the block description
-- aRawDesc is the description read from the palette file, "grass_block\tsnowy\tfalse"
-- Returns the normalized description "grass_block:snowy:false", where the block state keys are alpha-sorted
local function normalizeDesc(aRawDesc)
	local blockTypeName, blockStates = aRawDesc:match("(.-)\t(.*)")
	if not(blockTypeName) then
		-- There's no TABs in the raw desc, only simple block
		return aRawDesc
	end
	local blockStatesTab = {}
	local idx = 1
	blockStates = blockStates .. "\t"
	for k, v in blockStates:gmatch("(.-)\t(.-)\t") do
		blockStatesTab[idx] = k .. ":" .. v
		idx = idx + 1
	end
	table.sort(blockStatesTab)
	return blockTypeName .. ":" .. table.concat(blockStatesTab, ":")
end





--- Loads the 1.13 palette
--[[
Returns the palette loaded as a table:
{
	numberToDesc = {[1] = "minecraft:stone", [8] = "minecraft:grass_block:snowy:false", ...},
	descToNumber = {["minecraft:stone"] = 1, ...}
}
--]]
local function loadPalette()
	local pal = loadPalSplitHeader("../../Server/Protocol/1.13/base.btp.txt")
	local numberToDesc = {}
	local descToNumber = {}
	local prefix = pal.headers["CommonPrefix"] or ""
	for _, ln in ipairs(pal.data) do
		local number, desc = ln:match("(.-)\t(.*)")
		desc = normalizeDesc(desc)
		desc = prefix .. desc
		number = tonumber(number) or error("Index is not a number: " .. ln)
		numberToDesc[number] = desc
		descToNumber[desc] = number
	end
	return { numberToDesc = numberToDesc, descToNumber = descToNumber}
end





local function runCheck()
	-- Load the palettes:
	local pal = loadPalette()
	local upg = loadPalSplitHeader("../../Server/Protocol/UpgradeBlockTypePalette.txt")

	-- Check the UpgradePalette's BlockDescs against the 1.13 palette:
	local prefix = upg.headers["CommonPrefix"] or ""
	local notFound = {}
	local idx = 1
	local totalCnt = 0
	for _, ln in ipairs(upg.data) do
		local blockType, blockMeta, desc = ln:match("(.-)\t(.-)\t(.*)")
		local normDesc = normalizeDesc(prefix .. desc)
		if not(pal.descToNumber[normDesc]) then
			notFound[idx] = {blockType = blockType, blockMeta = blockMeta, desc = desc}
			idx = idx + 1
		end
		totalCnt = totalCnt + 1
	end
	idx = idx - 1

	-- Print the findings:
	print("Blocks not found: " .. idx .. " out of " .. totalCnt)
	for _, nf in ipairs(notFound) do
		print(nf.blockType .. "\t" .. nf.blockMeta .. "\t" .. nf.desc)
	end
	print("Blocks not found: " .. idx .. " out of " .. totalCnt)
end





-- MAIN:
runCheck()
