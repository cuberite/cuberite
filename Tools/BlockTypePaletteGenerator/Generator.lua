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

-- Parse the registry:
local registry = parseRegistry(io.input():read("*a"))
local commonPrefix = findCommonPrefix(registry)

-- Sort the entries:
table.sort(registry,
	function (entry1, entry2)
		return (entry1.id < entry2.id)
	end
)

-- Write out the output format:
io.write("BlockTypePalette\n")
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
		entry.id, "\t",
		string.sub(entry.blockTypeName, prefixLen),
		props, "\n"
	)
end
