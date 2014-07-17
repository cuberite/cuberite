
-- CheckBasicStyle.lua

--[[
Checks that all source files (*.cpp, *.h) use the basic style requirements of the project:
	- Tabs for indentation, spaces for alignment
	- Trailing whitespace on non-empty lines
	- Two spaces between code and line-end comment ("//")
	- (TODO) Spaces around +, -, (cannot check /, * or & due to their other usage - comment, ptr deref, address-of)
	- (TODO) Spaces before *, /, &
	- (TODO) Spaces after ,
	- (TODO) Hex numbers with even digit length
	- (TODO) Hex numbers in lowercase
	- (TODO) Braces not on the end of line
	- (TODO) Line dividers (////...) exactly 80 slashes
	
Reports all violations on stdout in a form that is readable by Visual Studio's parser, so that dblclicking
the line brings the editor directly to the violation.

Returns 0 on success, 1 on internal failure, 2 if any violations found

This script requires LuaFileSystem to be available in the current Lua interpreter.
--]]





-- Check that LFS is installed:
local hasLfs = pcall(require, "lfs")
if not(hasLfs) then
	print("This script requires LuaFileSystem to be installed")
	os.exit(1)
end
local lfs = require("lfs")
assert(lfs ~= nil)





-- The list of file extensions that are processed:
local g_ShouldProcessExt =
{
	["h"]   = true,
	["cpp"] = true,
}

--- The list of files not to be processed:
local g_IgnoredFiles =
{
	"./Bindings/Bindings.cpp",
	"./Bindings/DeprecatedBindings.cpp",
	"./LeakFinder.cpp",
	"./LeakFinder.h",
	"./MersenneTwister.h",
	"./StackWalker.cpp",
	"./StackWalker.h",
}

--- The list of files not to be processed, as a dictionary (filename => true), built from g_IgnoredFiles
local g_ShouldIgnoreFile = {}

-- Initialize the g_ShouldIgnoreFile map:
for _, fnam in ipairs(g_IgnoredFiles) do
	g_ShouldIgnoreFile[fnam] = true
end

--- Keeps track of the number of violations for this folder
local g_NumViolations = 0





--- Reports one violation
-- Pretty-prints the message
-- Also increments g_NumViolations
local function ReportViolation(a_FileName, a_LineNumber, a_Message)
	print(a_FileName .. "(" .. a_LineNumber .. "): " .. a_Message)
	g_NumViolations = g_NumViolations + 1
end





--- Processes one file
local function ProcessFile(a_FileName)
	assert(type(a_FileName) == "string")
	
	-- Read the whole file:
	local f, err = io.open(a_FileName, "r")
	if (f == nil) then
		print("Cannot open file \"" .. a_FileName .. "\": " .. err)
		print("Aborting")
		os.exit(1)
	end
	local all = f:read("*all")
	
	-- Check that the last line is empty - otherwise processing won't work properly:
	local lastChar = string.byte(all, string.len(all))
	if ((lastChar ~= 13) and (lastChar ~= 10)) then
		local numLines = 1
		string.gsub(all, "\n", function() numLines = numLines + 1 end)  -- Count the number of line-ends
		ReportViolation(a_FileName, numLines, "Missing empty line at file end")
		return
	end
	
	-- Process each line separately:
	-- Ref.: http://stackoverflow.com/questions/10416869/iterate-over-possibly-empty-lines-in-a-way-that-matches-the-expectations-of-exis
	local lineCounter = 1
	all:gsub("\r\n", "\n")  -- normalize CRLF into LF-only
	string.gsub(all .. "\n", "[^\n]*\n",  -- Iterate over each line, while preserving empty lines
		function(a_Line)
			-- Check against indenting using spaces:
			if (a_Line:find("^\t* +")) then  -- Find any number of tabs at the start of line (incl 0), followed by a space
				ReportViolation(a_FileName, lineCounter, "Indenting with a space")
			end
			-- Check against alignment using tabs:
			if (a_Line:find("[^%s]\t+[^%s]")) then  -- Find any number of tabs after non-whitespace followed by non-whitespace
				ReportViolation(a_FileName, lineCounter, "Aligning with a tab")
			end
			-- Check against trailing whitespace:
			if (a_Line:find("[^%s]%s+\n")) then  -- Find any whitespace after non-whitespace at the end of line
				ReportViolation(a_FileName, lineCounter, "Trailing whitespace")
			end
			-- Check that all "//"-style comments have at least two spaces in front (unless alone on line):
			if (a_Line:find("[^%s] //")) then
				ReportViolation(a_FileName, lineCounter, "Needs at least two spaces in front of a \"//\"-style comment")
			end
			-- Check that all "//"-style comments have at least one spaces after:
			if (a_Line:find("%s//[^%s/*<]")) then
				ReportViolation(a_FileName, lineCounter, "Needs a space after a \"//\"-style comment")
			end
			lineCounter = lineCounter + 1
		end
	)
end





--- Processes one item - a file or a folder
local function ProcessItem(a_ItemName)
	assert(type(a_ItemName) == "string")
	
	-- Skip files / folders that should be ignored
	if (g_ShouldIgnoreFile[a_ItemName]) then
		return
	end
	
	-- If the item is a folder, recurse:
	local attrs = lfs.attributes(a_ItemName)
	if (attrs and (attrs.mode == "directory")) then
		for fnam in lfs.dir(a_ItemName) do
			if ((fnam ~= ".") and (fnam ~= "..")) then
				ProcessItem(a_ItemName .. "/" .. fnam)
			end
		end
		return
	end
	
	local ext = a_ItemName:match("%.([^/%.]-)$")
	if (g_ShouldProcessExt[ext]) then
		ProcessFile(a_ItemName)
	end
end





-- Process the entire current folder:
ProcessItem(".")

-- Report final verdict:
print("Number of violations found: " .. g_NumViolations)
if (g_NumViolations > 0) then
	os.exit(2)
else
	os.exit(0)
end




