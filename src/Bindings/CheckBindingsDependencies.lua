-- CheckBindingsDependencies.lua

-- This script checks whether all files listed in AllToLua.pkg are also in the dependencies for bindings regeneration.
-- This script is run as part of the CircleCI tests





--- Filenames that should be ignored in the AllToLua.pkg file:
-- Dictionary of "filename" -> true for each ignored filename
local g_ShouldIgnorePkg =
{
	-- ["../IniFile.h"] = true,
}

--- Filenames that should be ignored in the CMakeLists.txt file:
-- Dictionary of "filename" -> true for each ignored filename
local g_ShouldIgnoreCMake =
{
	["tolua"] = true,
	["Bindings/AllToLua.pkg"] = true,
	["Bindings/BindingsProcessor.lua"] = true,
}





--- Returns a sorted list of all files listed in AllToLua.pkg
-- The returned table has both an array part (list of files) and a dictionary part ("filename" -> true)
local function getAllToLuaPkgFiles()
	local res = {}
	for line in io.lines("AllToLua.pkg") do
		line:gsub("$cfile \"(.+)\"",  -- Parse each line with a $cfile directive
			function (a_FileName)
				if (g_ShouldIgnorePkg[a_FileName]) then
					return
				end

				-- Normalize the path: AllToLua is relative to src\Bindings
				-- but the CMake dependencies list is relative to src\
				a_FileName, cnt = a_FileName:gsub("%.%./", "")

				-- If no replacements were done, this entry must point to a file
				-- inside the Bindings folder; normalize it
				if cnt == 0 then
					a_FileName = "Bindings/" .. a_FileName
				end

				table.insert(res, a_FileName)
				res[a_FileName] = true
			end
		)
	end
	table.sort(res)
	return res
end





--- Returns a sorted list of all files listed as dependencies in CMakeLists.txt
-- The returned table has both an array part (list of files) and a dictionary part ("filename" -> true)
local function getCMakeListsFiles()
	local f = assert(io.open("../../CMake/GenerateBindings.cmake", "r"))
	local contents = f:read("*all")
	f:close()
	local res = {}
	contents:gsub("set%s*(%b())",  -- Process each CMake's "set" statement
		function (a_SetStatement)
			if not(a_SetStatement:find("%(BINDING_DEPENDENCIES")) then
				return
			end
			-- This is the statement setting the dependencies, parse the files:
			a_SetStatement:gsub("%s(%S+)%s",
				function (a_FileName)
					if (g_ShouldIgnoreCMake[a_FileName]) then
						return
					end
					table.insert(res, a_FileName)
					res[a_FileName] = true
				end
			)
		end
	)
	table.sort(res)
	return res
end





-- Check each set of files against the other:
local pkgFiles = getAllToLuaPkgFiles()
local cmakeFiles = getCMakeListsFiles()
local numMissingFiles = 0
for _, fnam in ipairs(pkgFiles) do
	if not(cmakeFiles[fnam]) then
		io.stderr:write("Bindings dependency file ", fnam, " is not listed in src/Bindings/CMakeLists.txt\n")
		numMissingFiles = numMissingFiles + 1
	end
end
for _, fnam in ipairs(cmakeFiles) do
	if not(pkgFiles[fnam]) then
		io.stderr:write("Bindings dependency file ", fnam, " is not listed in src/Bindings/AllToLua.pkg\n")
		numMissingFiles = numMissingFiles + 1
	end
end

-- If any mismatch was found, exit with an error code:
if (numMissingFiles > 0) then
	io.stderr:write("Bindings dependency mismatches found: ", numMissingFiles, "\n")
	os.exit(1)
end
