-- Generator.lua

--[[
Crafts the LootTables.h file to be used by Cuberite.
It processes the loot_tables report folder (https://wiki.vg/Data_Generators)
into a the h file containing all LootTables

The output format is the regular LootTables.h
--]]





-- Allow Lua to load libraries in our subfolder:
package.path = '../BlockTypePaletteGenerator/lib/lunajson/src/?.lua;' .. package.path;





--- Prints usage instructions to stdout.
-- If the optional `aMessage` is passed, output is prepended by message _and_
-- redirected to stderr.
local function usage(aMessage)
	if aMessage then
		io.output(io.stderr);
		io.write(aMessage, "\n\n");
	end
	io.write(
		"Usage: lua Generator.lua INPUTFOLDER OUTPUTFILE\n"..
		"Converts the Minecraft loot_tables report format to the cuberite "..
		"LootTables.h.\n"..
		"\n"..
		"INPUTFOLDER and OUTPUTFILE must point to a valid path. INPUTFOLDER must "..
		"be readable and OUTPUTFILE must be writable. Either can be replaced "..
		"with `-` (dash character) to point to standard-input or -output.\n");
	os.exit(message and 1 or 0);
end



local function tchelper(first, rest)
	return first:upper()..rest:lower()
end

local function stringFixer(aString)
	aString = string.gsub(aString,"minecraft:","")
	aString = string.gsub(aString,"_", " ")
	aString = string.gsub(aString,"(%a)([%w_']*)", tchelper)
	aString = string.gsub(aString, " ", "")
	aString = string.gsub(aString,[["]],[[\"]])
	return aString
end


local function stringItemFixer(aString)
	aString = string.gsub(aString,"minecraft:","")
	aString = string.gsub(aString, "%f[%a]%l+%f[%A]", string.upper)
	aString = "cItem(E_ITEM_"..aString..")"
	
	aString = string.gsub(aString,"E_ITEM_IRON_INGOT","E_ITEM_IRON")
	aString = string.gsub(aString,"E_ITEM_GOLD_INGOT","E_ITEM_GOLD")
	aString = string.gsub(aString,"E_ITEM_REDSTONE","E_ITEM_REDSTONE_DUST")
	
	aString = string.gsub(aString,"E_ITEM_TORCH","E_BLOCK_TORCH")
	aString = string.gsub(aString,"E_ITEM_TNT","E_BLOCK_TNT")
	
	aString = string.gsub(aString,"E_ITEM_LEATHER_CHESTPLATE","E_ITEM_LEATHER_TUNIC")
	aString = string.gsub(aString,"E_ITEM_LEATHER_HELMET","E_ITEM_LEATHER_CAP")
	aString = string.gsub(aString,"E_ITEM_LEATHER_LEGGINGS","E_ITEM_LEATHER_PANTS")
	
	aString = string.gsub(aString,"E_ITEM_GOLDEN_HORSE_ARMOR","E_ITEM_GOLD_HORSE_ARMOR")
	aString = string.gsub(aString,"E_ITEM_GOLDEN_SWORD","E_ITEM_GOLD_SWORD")
	aString = string.gsub(aString,"E_ITEM_GOLDEN_CHESTPLATE","E_ITEM_GOLD_CHESTPLATE")
	aString = string.gsub(aString,"E_ITEM_GOLDEN_HELMET","E_ITEM_GOLD_HELMET")
	
	aString = string.gsub(aString,"E_ITEM_SAND","E_BLOCK_SAND")
	aString = string.gsub(aString,"E_ITEM_APPLE","E_ITEM_RED_APPLE")
	aString = string.gsub(aString,"E_ITEM_OBSIDIAN","E_BLOCK_OBSIDIAN")
	aString = string.gsub(aString,"E_ITEM_EXPERIENCE_BOTTLE","E_ITEM_BOTTLE_O_ENCHANTING")
	aString = string.gsub(aString,"E_ITEM_TRIPWIRE_HOOK","E_BLOCK_TRIPWIRE_HOOK")
	
	aString = string.gsub(aString,"E_ITEM_PUMPKIN","E_BLOCK_PUMPKIN")
	aString = string.gsub(aString,"E_BLOCK_PUMPKIN_SEEDS","E_ITEM_PUMPKIN_SEEDS")
	
	aString = string.gsub(aString,"E_ITEM_RAIL","E_BLOCK_RAIL")
	aString = string.gsub(aString,"E_ITEM_POWERED_RAIL","E_BLOCK_POWERED_RAIL")
	aString = string.gsub(aString,"E_ITEM_DETECTOR_RAIL","E_BLOCK_DETECTOR_RAIL")
	aString = string.gsub(aString,"E_ITEM_ACTIVATOR_RAIL","E_BLOCK_ACTIVATOR_RAIL")
	
	aString = string.gsub(aString,"E_ITEM_MUSIC_DISC_13","E_ITEM_13_DISC")
	aString = string.gsub(aString,"E_ITEM_MUSIC_DISC_CAT","E_ITEM_CAT_DISC")
	
	aString = string.gsub(aString,"E_ITEM_CHAINMAIL_CHESTPLATE","E_ITEM_CHAIN_CHESTPLATE")
	aString = string.gsub(aString,"E_ITEM_CHAINMAIL_CHESTPLATE","E_ITEM_CHAIN_CHESTPLATE")
	
	aString = string.gsub(aString,"E_ITEM_CHICKEN","E_ITEM_RAW_CHICKEN")
	aString = string.gsub(aString,"E_ITEM_MUTTON","E_ITEM_RAW_MUTTON")
	aString = string.gsub(aString,"E_ITEM_BEEF","E_ITEM_RAW_BEEF")
	aString = string.gsub(aString,"E_ITEM_PORKCHOP","E_ITEM_RAW_PORKCHOP")
	aString = string.gsub(aString,"E_ITEM_RABBIT","E_ITEM_RAW_RABBIT")
	
	aString = string.gsub(aString,"E_ITEM_SLIME_BALL","E_ITEM_SLIMEBALL")
	aString = string.gsub(aString,"E_ITEM_RABBIT_FOOT","E_ITEM_RABBITS_FOOT")
	aString = string.gsub(aString,"E_ITEM_RAW_RABBIT_HIDE","E_ITEM_RABBIT_HIDE")

	aString = string.gsub(aString,"E_ITEM_ENCHANTED_GOLDEN_APPLE","E_ITEM_GOLDEN_APPLE, 0x01")

	--[[
	E_META_HEAD_SKELETON = 0,
	E_META_HEAD_WITHER   = 1,
	E_META_HEAD_ZOMBIE   = 2,
	E_META_HEAD_PLAYER   = 3,
	E_META_HEAD_CREEPER  = 4,
	E_META_HEAD_DRAGON   = 5,
	--]]
	
	aString = string.gsub(aString,"E_ITEM_WITHER_SKELETON_SKULL","E_ITEM_HEAD, 1, 1")
	
	--[[  E_BLOCK_FLOWER
	E_META_FLOWER_POPPY           = 0,
	E_META_FLOWER_BLUE_ORCHID     = 1,
	E_META_FLOWER_ALLIUM          = 2,
	E_META_FLOWER_RED_TULIP       = 4,
	E_META_FLOWER_ORANGE_TULIP    = 5,
	E_META_FLOWER_WHITE_TULIP     = 6,
	E_META_FLOWER_PINK_TULIP      = 7,
	E_META_FLOWER_OXEYE_DAISY     = 8,
	--]]
	
	aString = string.gsub(aString,"E_ITEM_POPPY","E_BLOCK_FLOWER, 1, 0")
	
	--[[  E_ITEM_DYE
	E_META_DYE_BLACK      = 0,
	E_META_DYE_RED        = 1,
	E_META_DYE_GREEN      = 2,
	E_META_DYE_BROWN      = 3,
	E_META_DYE_BLUE       = 4,
	E_META_DYE_PURPLE     = 5,
	E_META_DYE_CYAN       = 6,
	E_META_DYE_LIGHTGRAY  = 7,
	E_META_DYE_GRAY       = 8,
	E_META_DYE_PINK       = 9,
	E_META_DYE_LIGHTGREEN = 10,
	E_META_DYE_YELLOW     = 11,
	E_META_DYE_LIGHTBLUE  = 12,
	E_META_DYE_MAGENTA    = 13,
	E_META_DYE_ORANGE     = 14,
	E_META_DYE_WHITE      = 15,
	--]]
	aString = string.gsub(aString,"E_ITEM_INK_SAC",     "E_ITEM_DYE")
	aString = string.gsub(aString,"E_ITEM_LAPIS_LAZULI","E_ITEM_DYE, 1, 4")
	aString = string.gsub(aString,"E_ITEM_BONE_MEAL",   "E_ITEM_DYE, 1, 15")
	
	
	--[[
	E_META_LOG_APPLE   = 0,
	E_META_LOG_CONIFER = 1,
	E_META_LOG_BIRCH   = 2,
	E_META_LOG_JUNGLE  = 3,
	--]]
	aString = string.gsub(aString,"E_ITEM_OAK_LOG",    "E_BLOCK_LOG")
	aString = string.gsub(aString,"E_ITEM_SPRUCE_LOG", "E_BLOCK_LOG, 1, 1")
	aString = string.gsub(aString,"E_ITEM_BIRCH_LOG",  "E_BLOCK_LOG, 1, 2")
	aString = string.gsub(aString,"E_ITEM_JUNGLE_LOG", "E_BLOCK_LOG, 1, 3")
	
	--[[
	E_META_NEW_LOG_ACACIA_WOOD   = 0,
	E_META_NEW_LOG_DARK_OAK_WOOD = 1,
	--]]
	aString = string.gsub(aString,"E_ITEM_ACACIA_LOG",  "E_BLOCK_NEW_LOG")
	aString = string.gsub(aString,"E_ITEM_DARK_OAK_LOG","E_BLOCK_NEW_LOG, 1, 1")
	
	--[[
	E_META_PLANKS_OAK      = 0,
	E_META_PLANKS_SPRUCE   = 1,
	E_META_PLANKS_BIRCH    = 2,
	E_META_PLANKS_JUNGLE   = 3,
	E_META_PLANKS_ACACIA   = 4,
	E_META_PLANKS_DARK_OAK = 5,
	--]]
	
	aString = string.gsub(aString,"E_ITEM_OAK_PLANKS","E_BLOCK_PLANKS")
	
	--[[
	E_META_RAW_FISH_FISH       = 0,
	E_META_RAW_FISH_SALMON     = 1,
	E_META_RAW_FISH_CLOWNFISH  = 2,
	E_META_RAW_FISH_PUFFERFISH = 3,
	--]]

	aString = string.gsub(aString,"E_ITEM_SALMON",         "E_ITEM_RAW_FISH, 1, 1")
	
	aString = string.gsub(aString,"E_ITEM_PUFFERFISH",     "E_ITEM_RAW_FISH, 1, 3")
	
	aString = string.gsub(aString,"E_ITEM_COD",            "E_ITEM_RAW_FISH")  --Todo: exchange for actual fish
	aString = string.gsub(aString,"E_ITEM_TROPICAL_FISH",  "E_ITEM_RAW_FISH")  --Todo: exchange for actual fish
	
	
	--[[
	E_META_COOKED_FISH_FISH       = 0,
	E_META_COOKED_FISH_SALMON     = 1,
	--]]
	
	aString = string.gsub(aString,"E_ITEM_COOKED_SALMON","E_ITEM_COOKED_FISH, 1, 1")
	aString = string.gsub(aString,"E_ITEM_COOKED_COD",   "E_ITEM_COOKED_FISH")  --Todo: exchange for actual fish
	
	--[[
	E_META_SPONGE_DRY              = 0,
	E_META_SPONGE_WET              = 1,
	--]]
	aString = string.gsub(aString,"E_ITEM_WET_SPONGE","E_BLOCK_SPONGE, 1, 1")
	
	return aString
end



local function parseLootTable(aLootTable, aName)
	-- read file name and create class definition
	assert(type(aLootTable) == "string")
	Name = string.gsub(aName, ".json","")
	Name = stringFixer(Name)
	io.write(
			"\n"..
			"class c"..Name.."LootTable: public cLootTable"..
			"\n{\n")
	-- check for json parser
	local lj = require("lunajson")
	local input = lj.decode(aLootTable)
	local rootTypeString, poolsString, poolString, rollString, poolStartString
	local weight, name, functionsString

	poolsString = ""
	poolStartString =
	"\tcLootTablePoolVector m_LootTablePools =\n"..
			"\t{\n"

	-- on the lowest layer you may have type, pools, functions
	for root, value in pairs(input) do
		if root == "type" then
			do
				rootTypeString = "\tenum LootTable::eType m_Type = LootTable::eType::"..stringFixer(value)..";\n"
			end
		elseif root == "pools" then
			do
				poolString = ""
				pools = value
				for _, pool in pairs(pools) do
					for entryType, entries in pairs(pool) do
						if entryType == "rolls" then
							do
								if type(entries) == "number" then
									do
										rollString = "cLootTablePoolRolls("..entries..", 0, 0)"
									end
								elseif type(entries) == "table" then
									do
										-- Todo: add type
										local min, max, disType
										for index, value in pairs(entries) do
											if index == "min" then do min = value end end
											if index == "max" then do max = value end end
											if index == "type" then do disType = value end end
										end
										rollString = "cLootTablePoolRolls(-1, "..min..", "..max..")"
									end
								end
							end
						elseif entryType == "entries" then
							do
								functionsString = ""
								functionParamString = ""
								functionCondString = ""
								functionTypeString = ""
								for _, entry in pairs(entries) do
									for index, value in pairs(entry) do
										weight = 0
										if index == "type" then
											do
												poolEntryType = value
											end
										elseif index == "name" then
											do
												name = stringItemFixer(value)
											end
										elseif index == "weight" then
											do
												weight = value
											end
										elseif index == "functions" then
											do
												for _,functions in pairs(value) do
													for functionsDescName, functionDesc in pairs(functions) do
														if functionsDescName == "function" then
															do
																functionTypeString = "LootTable::eFunctionType::"..stringFixer(functionDesc)
															end
														elseif functionsDescName == "conditions" then
															do
																for _,condition in pairs(functionDesc) do
																	for param, value in pairs(condition) do
																		if type(value) ~= "table" then
																			do
																				functionCondString = functionCondString.."{\""..param.."\", \""..stringFixer(value).."\"}, "
																			end
																		else
																			do
																				for _, flags in pairs(value) do
																					for a, b in pairs(flags) do
																						if type(b) == "boolean" then
																							if b then do b ="true" end
																							else do b ="false" end end
																						end
																						functionCondString = functionCondString.."{\""..a.."\", \""..b.."\"}, "
																					end
																				end
																			end
																		end
																	end
																end
															end -- conditions
														else -- values are function parameters
															do
																if type(functionDesc) == "boolean" then
																	if functionDesc then do functionDesc ="true" end
																	else do functionDesc ="false" end end
																end
																if type(functionDesc) ~= "table" then
																	do
																		functionParamString = functionParamString.."{\""..functionsDescName.."\", \""..stringFixer(functionDesc).."\"}, "
																	end
																else 
																	do
																		for param, val in pairs(functionDesc) do
																			if type(val) ~= "table" then
																				do
																					functionParamString = functionParamString.."{\""..param.."\", \""..stringFixer(val).."\"}, "
																				end
																			end
																		end
																	end
																end
															end
														end -- function parameters
													end -- function
													if functionTypeString ~= "" then
														do
															functionsString = functionsString.."cLootTableFunction("..functionTypeString..", {"..string.sub(functionParamString, 0, #functionParamString-2).."}, {"..string.sub(functionCondString, 0, #functionCondString-2).."}), "
															functionParamString = ""
															functionCondString = ""
															functionTypeString = ""
														end
													end
												end -- functions
											end
										end
									end -- index, value in entry
									if pool ~= "" then
										do
											if functionsString ~= "" then
												do
												poolString = poolString..
												"\t\t\t\tcLootTablePoolEntry(\n"..
												"\t\t\t\t\tcLootTableFunctionVector\n"..
												"\t\t\t\t\t{\n"..
												"\t\t\t\t\t\t"..string.sub(functionsString, 0, #functionsString-2)..
												"\n\t\t\t\t\t},\n"..
												"\t\t\t\t\t"..name..", "..weight.."\n\t\t\t\t),\n"
													functionsString = ""
												end
											else
												do
													poolString = poolString.. "\t\t\t\tcLootTablePoolEntry(".."{}, "..name..", "..weight.. "),\n"
												end
											end
										end
									end
								end -- entry in entries
								poolString = poolString.."\t\t\t}"
							end
						end -- entryType == "entries"
					end
						poolsString = poolsString.."\t\tcLootTablePool(\n\t\t\t"..rollString..",\n\t\t\tcLootTablePoolEntryVector\n\t\t\t{\n"..poolString.."\n\t\t),\n"
					rollString = ""
					poolString = ""
				end -- pool in pools
				poolsString = poolsString.."\t};\n"
			end
		end -- root == "type"
	end
	if rollString ~= nil then
		do
			io.write(rootTypeString)
			io.write(poolStartString..poolsString)
		end
	else
		do
			io.write("\tenum LootTable::eType m_Type = LootTable::eType::Empty;\n};\n")
			return
		end
	end
	io.write("};\n")
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
inpath = inpath or "loot_tables"
outpath = outpath or "LootTables.h"
--[[
if (inpath ~= "-") then
	local handle, err = io.open(inpath, "r")
	io.input(handle or usage(err))
end--]]
if (outpath ~= "-") then
	local handle, err = io.open(outpath, "w")
	io.output(handle or usage(err))
end
-- Parse the registry:
-- local registry = parseRegistry(io.input():read("*a"))
-- local commonPrefix = findCommonPrefix(registry)

io.write(
	"\n"..
	"// LootTables.h\n"..
	"\n"..
	"#pragma once\n"..
	"\n"..
	"#include \"../LootTableProvider.h\"\n\n"..
	"/*\n"..
	"This files contains all default loot tables\n"..
	"*/\n"
)

-- find all files in json format in specified folder
-- root dir
for dir in io.popen([[dir ".\]]..inpath..[[" /b /ad]]):lines() do
	if (dir ~= "blocks" and dir ~= "gameplay") then
		-- print(dir)
		for file in io.popen([[dir ".\]]..inpath..[[\]]..dir..[[" /b /a-d]]):lines() do
			-- print(inpath.."/"..dir.."/"..file)
			local handle, err = io.open([[.\]]..inpath..[[\]]..dir..[[\]]..file)
			io.input(handle or usage(err))
			parseLootTable(io.input():read("*a"), file)
		end

		for subdir in io.popen([[dir ".\]]..inpath..[[\]]..dir..[[" /b /ad]]):lines() do
			-- print(inpath.."/"..dir.."/"..subdir)
			for file in io.popen([[dir ".\]]..inpath..[[\]]..dir..[[" /b /a-d]]):lines() do
				-- print(inpath.."/"..dir.."/"..subdir.."/"..file)
			end
		end

	end
end

--[[
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
--]]