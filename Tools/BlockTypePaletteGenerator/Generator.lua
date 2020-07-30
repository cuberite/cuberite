-- Generator.lua

--[[
Crafts an intermediate block palette format to be read by Cuberite.
It processes the blocks.json report file (https://wiki.vg/Data_Generators)
into a file that can be loaded into a BlockTypePalette (and is to be stored
as Server/Protocol/<version>/base.btp.txt).

The output format is the regular TSV BlockTypePalette, described in the
$/src/BlockTypePalette.h file.
--]]




-- Allow Lua to load libraries in our subfolder:
package.path = 'lib/lunajson/src/?.lua;' .. package.path;




function spairs(t, order)
    -- collect the keys
    local keys = {}
    for k in pairs(t) do keys[#keys+1] = k end

    -- if order function given, sort by it by passing the table and keys a, b,
    -- otherwise just sort the keys
    if order then
        table.sort(keys, function(a,b) return order(t, a, b) end)
    else
        table.sort(keys)
    end

    -- return the iterator function
    local i = 0
    return function()
        i = i + 1
        if keys[i] then
            return keys[i], t[keys[i]]
        end
    end
end



--- Prints usage instructions to stdout.
-- If the optional `aMessage` is passed, output is prepended by message _and_
-- redirected to stderr.
local function usage(aMessage)
	if aMessage then
		io.output(io.stderr);
		io.write(aMessage, "\n\n");
	end
	io.write(
		"Usage: lua Generator.lua INPUTFILE OUTPUTFILE\n"..
		"Converts the Minecraft blocks.json report format to the cuberite "..
		"block type palette format.\n"..
		"\n"..
		"INPUTFILE and OUTPUTFILE must point to a valid path. INPUTFILE must "..
		"be readable and OUTPUTFILE must be writable. Either can be replaced "..
		"with `-` (dash character) to point to standard-input or -output.\n");
	os.exit(message and 1 or 0);
end





--- Parses the JSON registry into a Lua table
--[[ The returned array-table has the following format:
{
	{ id = 1, blockTypeName = "minecraft:stone", properties = {key = value, ...} },
	...
}
--]]
local function parseRegistry(aBlockRegistryJsonStr)
	assert(type(aBlockRegistryJsonStr) == "string")

	local lj = require("lunajson")
	local input = lj.decode(aBlockRegistryJsonStr)
	local registry = {}
	local idx = 1
	for blockTypeName, blockData in pairs(input) do
		local function tchelper(before, underscore, first)
			return before..first:upper()
		end

		blockTypeName = blockTypeName:gsub("(%a)([_:])(%a)", tchelper)
		print(blockTypeName)

		for _, state in pairs(blockData.states) do
			registry[idx] = {
				id = state.id,
				blockTypeName = blockTypeName,
				properties = state.properties,
			}
			idx = idx + 1
		end
	end
	return registry
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
	usage(
		"Could not load required libraries, please run `Generator.lua` " ..
		"within its directory and make sure to run `git submodule update`."
	)
end

-- Check/Prepare CLI arguments
local inpath, outpath = ...;
inpath = inpath or "blocks.json"
outpath = outpath or "base.btp.txt"
if (inpath ~= "-") then
	local handle, err = io.open(inpath, "r")
	io.input(handle or usage(err))
end
if (outpath ~= "-") then
	local handle, err = io.open(outpath, "w")
	io.output(handle or usage(err))
end

aBlockRegistryJsonStr = io.input():read("*a")
assert(type(aBlockRegistryJsonStr) == "string")

local function makeTitleCase(input)
	local function tchelper(before, underscore, first)
		return before..first:upper()
	end

	if (input == "hay_block") then
		return "HayBale"
	elseif (input == "tnt") then
		return "TNT"
	end

	input = input:gsub("(%a)(_)(%a)", tchelper)
	input = input:gsub("(%a)(_)(%a)", tchelper)
	return (input:gsub("^%l", string.upper))
end

local input = require("lunajson").decode(aBlockRegistryJsonStr)
local registry = {}
header = true

for blockTypeName, blockData in pairs(input) do

	local default = 0
	local properties = {}
	local states = {}

	blockTypeName = makeTitleCase(blockTypeName:gsub("minecraft:", "", 1))

	if blockData.properties ~= nil then
		for property, values in pairs(blockData.properties) do

			local firstProperty = values[1]
			local property = makeTitleCase(property)
			local propertyType

			if firstProperty == "true" then
				propertyType = "bool"
			elseif tonumber(firstProperty) ~= nil then
				propertyType = "unsigned char"
			elseif (firstProperty == "north") or (firstProperty == "down") then -- Down is special for Hopper
				propertyType = "eBlockFace"
			else
				propertyType = {}
				for _, value in pairs(values) do
					table.insert(propertyType, makeTitleCase(value))
				end
			end

			if property ~= "Waterlogged" then
				properties[property] = propertyType
			end
		end
	end

	for _, state in pairs(blockData.states) do

		local stateForThisId = {}
		states[state.id] = stateForThisId

		if state.default ~= nil then
			default = state.id
		end

		if state.properties ~= nil then
			for property, value in pairs(state.properties) do

				local property = makeTitleCase(property)

				if (property == "Waterlogged") then
					if (value == "true") then
						states[state.id] = nil
					end
				elseif type(properties[property]) == "table" then
					stateForThisId[property] = property .. "::" .. makeTitleCase(value)
				elseif (properties[property] == "eBlockFace") then
					local faceMapping = {
						["north"] = "eBlockFace::BLOCK_FACE_ZM",
						["south"] = "eBlockFace::BLOCK_FACE_ZP",
						["west"] = "eBlockFace::BLOCK_FACE_XM",
						["east"] = "eBlockFace::BLOCK_FACE_XP",
						["up"] = "eBlockFace::BLOCK_FACE_YP",
						["down"] = "eBlockFace::BLOCK_FACE_YM"
					}
					stateForThisId[property] = faceMapping[value]
				else
					stateForThisId[property] = value
				end
			end
		end
	end

	registry[blockTypeName] = { default, properties, states }
end

local function writeBlocksEnum(registry)
	if not header then
		return
	end

	local blockTypeNames = {}
	for blockTypeName, _ in spairs(registry) do
		table.insert(blockTypeNames, blockTypeName)
	end
	io.write(
		"\nenum class Type\n{\n\t",
		table.concat(blockTypeNames, ",\n\t"),
		"\n};"
	)
end

local function writeTypeGetter(registry)
	io.write("\nenum Type Type(short ID)")

	if header then
		io.write(";")
		return
	end

	io.write("\n{\n\tswitch(ID)\n\t{\n\t")

	local lastBlockIndex = 0
	for _, _ in pairs(registry) do
		lastBlockIndex = lastBlockIndex + 1
	end

	local index = 0
	local previousTypeName, _ = spairs(registry)()
	for blockTypeName, blockData in spairs(registry) do
		if (previousTypeName ~= blockTypeName) then
			io.write(" return Type::", previousTypeName, ";\n\t")
			previousTypeName = blockTypeName
		end

		index = index + 1
		if (index == lastBlockIndex) then
			io.write("default: return Type::", blockTypeName, ";\n\t}\n}")
			return
		end

		for stateId, _ in spairs(blockData[3]) do
			io.write("case ", stateId, ":")
		end
	end
end

local function writeTypeTester()
	io.write("\nbool Is(short ID, enum Type Type)")
	if header then
		io.write(";")
	else
		io.write("\n{\n\treturn Block::Type(ID) == Type;\n}")
	end
end

local function writeStateTypes(blockTypeName, properties)
	if not header then
		return
	end

	for property, propertyType in spairs(properties) do
		if type(propertyType) == "table" then
			io.write(
				"\nenum class ", property, "\n{\n\t",
				table.concat(propertyType, ",\n\t"),
				"\n};"
			)
		end
	end
end

local function writeDefaultSerialiser(blockTypeName, default)
	io.write("\nshort ", blockTypeName, "()")
	if header then
		io.write(";")
	else
		io.write("\n{\n\treturn ", default, ";\n}")
	end
end

local function adjustMaybeEnumType(property, propertyType)
	if type(propertyType) == "table" then
		return "enum " .. property
	else
		return propertyType
	end
end

local function writeSerialisers(blockTypeName, properties, states)
	if not header then
		local argCount = 0
		for property, _ in spairs(properties) do
			argCount = argCount + 1
		end
		return argCount
	end

	local argCount = 0
	local arguments = {}
	for property, propertyType in spairs(properties) do
		argCount = argCount + 1
		local adjustedType = adjustMaybeEnumType(property, propertyType)
		table.insert(arguments, adjustedType .. " " .. property)
	end

	io.write(
		"\nconstexpr short ", blockTypeName,
		"(", table.concat(arguments, ", "), ")\n{"
	)

	local lastStateIndex = 0
	for _, _ in pairs(states) do
		lastStateIndex = lastStateIndex + 1
	end

	local index = 0
	for stateId, values in spairs(states) do
		local equalities = {}
		for property, value in spairs(values) do
			if (value == "true") then
				table.insert(equalities, property)
			elseif (value == "false") then
				table.insert(equalities, "!" .. property)
			else
				table.insert(equalities, property .. " == " .. value)
			end
		end

		index = index + 1
		if (index == lastStateIndex) then
			io.write("\n\treturn ", stateId, ";\n}")
			return argCount
		else
			io.write("\n\tif (", table.concat(equalities, " && "), ") return ", stateId, ";")
		end
	end
end

local function writeGetters(properties, states)
	for property, propertyType in spairs(properties) do
		local adjustedType = adjustMaybeEnumType(property, propertyType)
		io.write("\n", adjustedType, " ", property, "(short ID)")

		if header then
			io.write(";")
		else
			io.write("\n{\n\tswitch(ID)\n\t{\n\t")

			local valuesArray = {}
			local lastValuesArrayIndex = 0

			for stateId, propertyValuePairs in spairs(states) do
				if (valuesArray[propertyValuePairs[property]] == nil) then
					valuesArray[propertyValuePairs[property]] = {}
					lastValuesArrayIndex = lastValuesArrayIndex + 1
				end
				table.insert(valuesArray[propertyValuePairs[property]], stateId)
			end

			local index = 0
			local previousValue, _ = spairs(valuesArray)()
			for value, stateIds in spairs(valuesArray) do
				index = index + 1
				if (index == lastValuesArrayIndex) then
					io.write("default: return ", value, ";\n\t}\n}")
				else
					for _, stateId in spairs(stateIds) do
						io.write("case ", stateId, ": ")
					end
					io.write("return ", value, ";\n\t")
				end
			end
		end
	end
end

local function writeBlocks(registry)
	io.write("\nnamespace Block\n{")
	writeBlocksEnum(registry)
	writeTypeGetter(registry)
	writeTypeTester()

	for blockTypeName, blockData in spairs(registry) do
		io.write("\nnamespace ", blockTypeName, "\n{")

		local default = blockData[1]
		local properties = blockData[2]
		local states = blockData[3]

		writeStateTypes(blockTypeName, properties)
		if (writeSerialisers(blockTypeName, properties, states) > 0) then
			writeDefaultSerialiser(blockTypeName, default)
		end
		writeGetters(properties, states)

		io.write("\n}")
	end
	io.write("\n}")
end

local function writeGlobalPalette(registry)
	io.write("UInt32 FromBlock(short ID)\n{\n\tusing namespace Block;\n\n\tswitch (ID)\n\t{")
	for blockTypeName, blockData in spairs(registry) do
		local states = blockData[3]
		for stateId, propertyValuePairs in spairs(states) do
			local values = {}
			for property, value in spairs(propertyValuePairs) do
				if (value == "true") or (value == "false") or (tonumber(value) ~= nil) or (string.sub(value,1,string.len("eBlockFace"))=="eBlockFace") then
					table.insert(values, value)
				else
					table.insert(values, blockTypeName .. "::" .. value)
				end
			end
			io.write(
				"\n\tcase ", blockTypeName, "::", blockTypeName, "(", table.concat(values, ","),
				"): return ", stateId, ";"
			)
		end
	end
	io.write("\n\tdefault: return 0;\n\t}\n}")
end

if false then
	writeBlocks(registry)
else
	writeGlobalPalette(registry)
end
