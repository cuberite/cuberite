-- UpgradeGenerator.lua

--[[ Creates the UpgradeBlockTypePalette out of JSON data of the Minutor project
(https://github.com/mrkite/minutor/blob/master/definitions/vanilla_ids.json

Parses the JSON into memory, then walks each block's "id" member and possibly
the "variants" sub-member to read the block types. The name is either present as "flatname",
or is synthesized from the internal Minutor "name" by lowercasing and replacing spaces
with underscores.

Expects two parameters, the input file and output file; either can be replaced by
a "-" to use stdin / stdout instead. If not given, the input file defaults to
"vanilla_ids.json" and the output file defaults to "UpgradeBlockTypePalette.txt"

The output format is the upgrade TSV BlockTypePalette, described in the
$/src/BlockTypePalette.h file.
--]]






-- Allow Lua to load libraries in our subfolder:
package.path = 'lib/lunajson/src/?.lua;' .. package.path;






--- Splits the full flat name into flat name and properties
-- "minecraft:carrots:age:0" -> "minecraft:carrots", {age = 0}
local function splitFlatName(aFullFlatName)
	local props = {}
	local numParts = 0
	local flatName = ""
	local propKey = ""
	aFullFlatName:gsub("([^:]+)",
		function (aPart)
			if (numParts == 0) then
				flatName = aPart
			elseif (numParts == 1) then
				flatName = flatName .. ":" .. aPart
			elseif (numParts % 2 == 0) then
				propKey = aPart
			else
				props[propKey] = aPart
			end
			numParts = numParts + 1
		end
	)
	return flatName, props
end





--- Returns the minecraft block name, created from the flat name if present, or synthesized
-- from the Minutor name
-- If the flat name contains encoded block properties, it returns those properties as a dict-table
-- in the second return value
local function processBlockName(aFlatName, aMinutorName)
	if (aFlatName) then
		assert(type(aFlatName) == "string")
		return splitFlatName(aFlatName)
	end
	if not(type(aMinutorName) == "string") then
		return nil
	end
	return "minecraft:" .. (aMinutorName:lower():gsub(" ", "_")), {}
end






--- Serializes the properties from the JSON / array table format into a single output string
-- Concatenates all properties with \t as the delimiting character
local function serializeProperties(aProperties)
	local res = {}
	local idx = 1
	for k, v in pairs(aProperties or {}) do
		res[idx] = k
		res[idx + 1] = v
		idx = idx + 2
	end
	return table.concat(res, "\t")
end





--- Parses the vanilla_ids.json into a common registry format
-- The returned registry is an array-table of
-- {blockType = 1, blockMeta = 2, blockTypeName = "name", properties = {key = value, ...}}
local function parseRegistry(aJsonString)
	assert(type(aJsonString) == "string")

	-- Parse the JSON:
	local lj = require("lunajson")
	local input = lj.decode(aJsonString)
	if (not(input) or (input["type"] ~= "block") or not(input["data"])) then
		error("The input file doesn't contain vanilla IDs.")
	end

	-- Create the registry:
	local registry = {}
	local idx = 1
	for _, entry in pairs(input["data"]) do
		local id = entry["id"]
		local parentBlockTypeName, props = processBlockName(entry["flatname"], entry["name"])
		registry[idx] =
		{
			blockType = id,
			blockMeta = 0,
			blockTypeName = parentBlockTypeName,
			properties = props,
		}
		idx = idx + 1
		for _, variant in pairs(entry["variants"] or {}) do
			local blockTypeName, props = processBlockName(variant["flatname"], variant["name"])
			if not(blockTypeName) then
				-- Some blocks don't have all their variants named ("brown mushroom block"), use the parent name in such a case
				blockTypeName = parentBlockTypeName
			end
			registry[idx] =
			{
				blockType = id,
				blockMeta = variant["data"],
				blockTypeName = blockTypeName,
				properties = props,
			}
			idx = idx + 1
		end
	end
	return registry
end





--- Returns the prefix that is common for all block type names in the registry
-- aRegistry is the parsed registry, as returned from parseRegistry()
local function findCommonPrefix(aRegistryTable)
	local prefix = aRegistryTable[1].blockTypeName
	local len = string.len(prefix)
	local sub = string.sub
	for _, block in ipairs(aRegistryTable) do
		while (sub(block.blockTypeName, 1, len) ~= prefix) do
			len = len - 1
			if (len == 0) then
				return ""
			end
			prefix = sub(prefix, 1, len)
		end
	end
	return prefix
end





-- Test whether the script is run in a path where it can load it's libraries
if not(pcall(function() require("lunajson") end)) then
	error(
		"Could not load required libraries, please run `UpgradeGenerator.lua` " ..
		"within its directory and make sure to run `git submodule update`."
	)
end

-- Check/Prepare CLI arguments
local inpath, outpath = ...;
inpath = inpath or "vanilla_ids.json"
outpath = outpath or "UpgradeBlockTypePalette.txt"
if (inpath ~= "-") then
	local handle, err = io.open(inpath, "r")
	io.input(handle or usage(err))
end
if (outpath ~= "-") then
	local handle, err = io.open(outpath, "w")
	io.output(handle or usage(err))
end

-- Parse the registry:
local registry = parseRegistry(io.input():read("*a"))
local commonPrefix = findCommonPrefix(registry)

-- Sort the entries:
table.sort(registry,
	function (entry1, entry2)
		if (entry1.blockType < entry2.blockType) then
			return true
		elseif (entry1.blockType > entry2.blockType) then
			return false
		else
			return (entry1.blockMeta < entry2.blockMeta)
		end
	end
)

-- Write out the output format:
io.write("UpgradeBlockTypePalette\n")
io.write("FileVersion\t1\n")
io.write("CommonPrefix\t", commonPrefix, "\n")
io.write("\n")
local prefixLen = string.len(commonPrefix) + 1
for _, entry in ipairs(registry) do
	local props = serializeProperties(entry.properties)
	if (props ~= "") then
		props = "\t" .. props
	end
	io.write(
		entry.blockType, "\t", entry.blockMeta, "\t",
		string.sub(entry.blockTypeName, prefixLen),
		props, "\n"
	)
end
