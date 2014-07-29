#!/usr/bin/env lua

-- CheckBasicStyle.lua

--[[
Checks that all source files (*.cpp, *.h) use the basic style requirements of the project:
	- Tabs for indentation, spaces for alignment
	- Trailing whitespace on non-empty lines
	- Two spaces between code and line-end comment ("//")
	- Spaces after comma, not before
	- Opening braces not at the end of a code line
	- Spaces after if, for, while
	- (TODO) Spaces before *, /, &
	- (TODO) Hex numbers with even digit length
	- (TODO) Hex numbers in lowercase
	- (TODO) Line dividers (////...) exactly 80 slashes
	- (TODO) Not using "* "-style doxy comment continuation lines
	
Violations that cannot be checked easily:
	- Spaces around "+" (there are things like "a++", "++a", "a += 1", "X+", "stack +1" and ascii-drawn tables)
	
Reports all violations on stdout in a form that is readable by Visual Studio's parser, so that dblclicking
the line brings the editor directly to the violation.

Returns 0 on success, 1 on internal failure, 2 if any violations found

--]]





-- The list of file extensions that are processed:
local g_ShouldProcessExt =
{
	["h"]   = true,
	["cpp"] = true,
}

--- The list of files not to be processed:
local g_IgnoredFiles =
{
	"Bindings/Bindings.cpp",
	"LeakFinder.cpp",
	"LeakFinder.h",
	"MersenneTwister.h",
	"StackWalker.cpp",
	"StackWalker.h",
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
local function ReportViolation(a_FileName, a_LineNumber, a_PatStart, a_PatEnd, a_Message)
	print(a_FileName .. "(" .. a_LineNumber .. "): " .. a_PatStart .. " .. " .. a_PatEnd .. ": " .. a_Message)
	g_NumViolations = g_NumViolations + 1
end





--- Searches for the specified pattern, if found, reports it as a violation with the given message
local function ReportViolationIfFound(a_Line, a_FileName, a_LineNum, a_Pattern, a_Message)
	local patStart, patEnd = a_Line:find(a_Pattern)
	if not(patStart) then
		return
	end
	ReportViolation(a_FileName, a_LineNum, patStart, patEnd, a_Message)
end





local g_ViolationPatterns =
{
	-- Check against indenting using spaces:
	{"^\t* +", "Indenting with a space"},
	
	-- Check against alignment using tabs:
	{"[^%s]\t+[^%s]", "Aligning with a tab"},
	
	-- Check against trailing whitespace:
	{"[^%s]%s+\n", "Trailing whitespace"},
	
	-- Check that all "//"-style comments have at least two spaces in front (unless alone on line):
	{"[^%s] //", "Needs at least two spaces in front of a \"//\"-style comment"},
	
	-- Check that all "//"-style comments have at least one spaces after:
	{"%s//[^%s/*<]", "Needs a space after a \"//\"-style comment"},
	
	-- Check that all commas have spaces after them and not in front of them:
	{" ,", "Extra space before a \",\""},
	{",[^%s\"%%]", "Needs a space after a \",\""},  -- Report all except >> "," << needed for splitting and >>,%s<< needed for formatting
	
	-- Check that opening braces are not at the end of a code line:
	{"[^%s].-{\n?$", "Brace should be on a separate line"},
	
	-- Space after keywords:
	{"[^_]if%(", "Needs a space after \"if\""},
	{"for%(", "Needs a space after \"for\""},
	{"while%(", "Needs a space after \"while\""},
	{"switch%(", "Needs a space after \"switch\""},
	{"catch%(", "Needs a space after \"catch\""},
	
	-- No space after keyword's parenthesis:
	{"[^%a#]if %( ", "Remove the space after \"(\""},
	{"for %( ", "Remove the space after \"(\""},
	{"while %( ", "Remove the space after \"(\""},
	{"catch %( ", "Remove the space after \"(\""},
	
	-- No space before a closing parenthesis:
	{" %)", "Remove the space before \")\""},
}





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
		ReportViolation(a_FileName, numLines, 1, 1, "Missing empty line at file end")
		return
	end
	
	-- Process each line separately:
	-- Ref.: http://stackoverflow.com/questions/10416869/iterate-over-possibly-empty-lines-in-a-way-that-matches-the-expectations-of-exis
	local lineCounter = 1
	all:gsub("\r\n", "\n")  -- normalize CRLF into LF-only
	string.gsub(all .. "\n", "[^\n]*\n",  -- Iterate over each line, while preserving empty lines
		function(a_Line)
			-- Check against each violation pattern:
			for _, pat in ipairs(g_ViolationPatterns) do
				ReportViolationIfFound(a_Line, a_FileName, lineCounter, pat[1], pat[2])
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
	
	local ext = a_ItemName:match("%.([^/%.]-)$")
	if (g_ShouldProcessExt[ext]) then
		ProcessFile(a_ItemName)
	end
end





-- Process all files in the AllFiles.lst file (generated by cmake):
for fnam in io.lines("AllFiles.lst") do
	ProcessItem(fnam)
end

-- Report final verdict:
print("Number of violations found: " .. g_NumViolations)
if (g_NumViolations > 0) then
	os.exit(2)
else
	os.exit(0)
end




