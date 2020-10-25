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
	- Line dividers (////...) exactly 80 slashes
	- Multi-level indent change
	- (TODO) Spaces before *, /, &
	- (TODO) Hex numbers with even digit length
	- (TODO) Hex numbers in lowercase
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
	"Bindings/Bindings.h",
	"Bindings/Bindings.cpp",
	"Bindings/LuaState_Implementation.cpp",
	"Registries/Blocks.h"
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
	-- Parenthesis around comparisons:
	{"==[^)]+&&",     "Add parenthesis around comparison"},
	{"&&[^(]+==",     "Add parenthesis around comparison"},
	{"==[^)]+||",     "Add parenthesis around comparison"},
	{"||[^(]+==",     "Add parenthesis around comparison"},
	{"!=[^)]+&&",     "Add parenthesis around comparison"},
	{"&&[^(]+!=",     "Add parenthesis around comparison"},
	{"!=[^)]+||",     "Add parenthesis around comparison"},
	{"||[^(]+!=",     "Add parenthesis around comparison"},
	{"<[^)>]*&&",     "Add parenthesis around comparison"},  -- Must take special care of templates: "template <T> fn(Args && ...)"
	-- Cannot check a < following a && due to functions of the form x fn(y&& a, z<b> c)
	{"<[^)>]*||",     "Add parenthesis around comparison"},  -- Must take special care of templates: "template <T> fn(Args && ...)"
	{"||[^(]+<",      "Add parenthesis around comparison"},
	-- Cannot check ">" because of "obj->m_Flag &&". Check at least ">=":
	{">=[^)]+&&",     "Add parenthesis around comparison"},
	{"&&[^(]+>=",     "Add parenthesis around comparison"},
	{">=[^)]+||",     "Add parenthesis around comparison"},
	{"||[^(]+>=",     "Add parenthesis around comparison"},

	-- Check against indenting using spaces:
	{"^\t* +", "Indenting with a space"},

	-- Check against alignment using tabs:
	{"[^%s]\t+[^%s]", "Aligning with a tab"},

	-- Check against trailing whitespace:
	{"%s+\n", "Trailing whitespace or whitespace-only line"},

	-- Check that all "//"-style comments have at least two spaces in front (unless alone on line):
	{"[^%s] //", "Needs at least two spaces in front of a \"//\"-style comment"},

	-- Check that all "//"-style comments have at least one spaces after:
	{"%s//[^%s/*<]", "Needs a space after a \"//\"-style comment"},

	-- Check that doxy-comments are used only in the double-asterisk form:
	{"/// ", "Use doxycomments in the form /** Comment */"},

	-- Check that /* */ comments have whitespace around the insides:
	{"%*%*/",        "Wrong comment termination, use */"},
	{"/%*[^%s*/\"]", "Needs a space after /*"},  -- Need to take care of the special "//*/" comment ends
	{"/%*%*[^%s*<]", "Needs a space after /**"},
	{"[^%s/*]%*/",   "Needs a space before */"},

	-- Check against MS XML doxycomments:
	{"/%*%* <", "Remove the MS XML markers from comment"},

	-- Check that all commas have spaces after them and not in front of them:
	{" ,", "Extra space before a \",\""},
	{",[^%s\"%%\']", "Needs a space after a \",\""},  -- Report all except >> "," << needed for splitting and >>,%s<< needed for formatting

	-- Check that opening braces are not at the end of a code line:
	{"[^%s].-{\n?$", "Brace should be on a separate line"},

	-- Space after keywords:
	{"[^_]if%(", "Needs a space after \"if\""},
	{"%sfor%(", "Needs a space after \"for\""},
	{"%swhile%(", "Needs a space after \"while\""},
	{"%sswitch%(", "Needs a space after \"switch\""},
	{"%scatch%(", "Needs a space after \"catch\""},
	{"%stemplate<", "Needs a space after \"template\""},

	-- No space after keyword's parenthesis:
	{"[^%a#]if %( ", "Remove the space after \"(\""},
	{"for %( ", "Remove the space after \"(\""},
	{"while %( ", "Remove the space after \"(\""},
	{"catch %( ", "Remove the space after \"(\""},

	-- No space before a closing parenthesis:
	{" %)", "Remove the space before \")\""},

	-- Check spaces around "+":
	{"^[a-zA-Z0-9]+%+[a-zA-Z0-9]+",                      "Add space around +"},
	{"[!@#$%%%^&*() %[%]\t][a-zA-Z0-9]+%+[a-zA-Z0-9]+",  "Add space around +"},
	--[[
	-- Cannot check these because of text such as "X+" and "+2" appearing in some comments.
	{"^[a-zA-Z0-9]+ %+[a-zA-Z0-9]+",                     "Add space after +"},
	{"[!@#$%%%^&*() %[%]\t][a-zA-Z0-9]+ %+[a-zA-Z0-9]+", "Add space after +"},
	{"^[a-zA-Z0-9]+%+ [a-zA-Z0-9]+",                     "Add space before +"},
	{"[!@#$%%%^&*() %[%]\t][a-zA-Z0-9]+%+ [a-zA-Z0-9]+", "Add space before +"},
	--]]

	-- Cannot check spaces around "-", because the minus is sometimes used as a hyphen between-words

	-- Check spaces around "*":
	{"^[a-zA-Z0-9]+%*[a-zA-Z0-9]+",                             "Add space around *"},
	{"^[^\"]*[!@#$%%%^&*() %[%]\t][a-zA-Z0-9]+%*[a-zA-Z0-9]+",  "Add space around *"},
	{"^[a-zB-Z0-9]+%* [a-zA-Z0-9]+",                            "Add space before *"},
	{"^[^\"]*[!@#$%%%^&*() %[%]\t][a-zB-Z0-9]+%* [a-zA-Z0-9]+", "Add space before *"},

	-- Check spaces around "/":
	{"^[a-zA-Z0-9]+%/[a-zA-Z0-9]+",                            "Add space around /"},
	{"^[^\"]*[!@#$%%%^&*() %[%]\t][a-zA-Z0-9]+%/[a-zA-Z0-9]+", "Add space around /"},

	-- Check spaces around "&":
	{"^[a-zA-Z0-9]+%&[a-zA-Z0-9]+",                             "Add space around &"},
	{"^[^\"]*[!@#$%%%^&*() %[%]\t][a-zA-Z0-9]+%&[a-zA-Z0-9]+",  "Add space around &"},
	{"^[a-zA-Z0-9]+%& [a-zA-Z0-9]+",                            "Add space before &"},
	{"^[^\"]*[!@#$%%%^&*() %[%]\t][a-zA-Z0-9]+%& [a-zA-Z0-9]+", "Add space before &"},

	-- Check spaces around "==", "<=" and ">=":
	{"==[a-zA-Z0-9]+",                             "Add space after =="},
	{"[a-zA-Z0-9]+==[^\\]",                        "Add space before =="},
	{"<=[a-zA-Z0-9]+",                             "Add space after <="},
	{"[a-zA-Z0-9]+<=",                             "Add space before <="},
	{">=[a-zA-Z0-9]+",                             "Add space after >="},
	{"[a-zA-Z0-9]+>=",                             "Add space before >="},

	-- We don't like "Type const *" and "Type const &". Use "const Type *" and "const Type &" instead:
	{"const %&", "Use 'const Type &' instead of 'Type const &'"},
	{"const %*", "Use 'const Type *' instead of 'Type const *'"},

	-- Check if "else" is on the same line as a brace.
	{"}%s*else", "else has to be on a separate line"},
	{"else%s*{", "else has to be on a separate line"},

	-- Don't allow characters other than ASCII 0 - 127:
	{"[" .. string.char(128) .. "-" .. string.char(255) .. "]", "Character in the extended ASCII range (128 - 255) not allowed"},
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
	f:close()

	-- Check that the last line is empty - otherwise processing won't work properly:
	local lastChar = string.byte(all, string.len(all))
	if ((lastChar ~= 13) and (lastChar ~= 10)) then
		local numLines = 1
		string.gsub(all, "\n", function() numLines = numLines + 1 end)  -- Count the number of line-ends
		ReportViolation(a_FileName, numLines, 1, 1, "Missing empty line at file end")
		return
	end

	-- Process each line separately:
	-- Ref.: https://stackoverflow.com/questions/10416869/iterate-over-possibly-empty-lines-in-a-way-that-matches-the-expectations-of-exis
	local lineCounter = 1
	local lastIndentLevel = 0
	local isLastLineControl = false
	local lastNonEmptyLine = 0
	local isAfterFunction = false
	local isSourceFile = a_FileName:match("%.cpp")
	all = all:gsub("\r\n", "\n")  -- normalize CRLF into LF-only
	string.gsub(all .. "\n", "[^\n]*\n",  -- Iterate over each line, while preserving empty lines
		function(a_Line)
			-- Check against each violation pattern:
			for _, pat in ipairs(g_ViolationPatterns) do
				ReportViolationIfFound(a_Line, a_FileName, lineCounter, pat[1], pat[2])
			end

			-- Check that divider comments are well formed - 80 slashes plus optional indent:
			local dividerStart, dividerEnd = a_Line:find("/////*")
			if (dividerStart) then
				if (dividerEnd ~= dividerStart + 79) then
					ReportViolation(a_FileName, lineCounter, 1, 80, "Divider comment should have exactly 80 slashes")
				end
				if (dividerStart > 1) then
					if (
						(a_Line:sub(1, dividerStart - 1) ~= string.rep("\t", dividerStart - 1)) or  -- The divider should have only indent in front of it
						(a_Line:len() > dividerEnd + 1)                                             -- The divider should have no other text following it
					) then
						ReportViolation(a_FileName, lineCounter, 1, 80, "Divider comment shouldn't have any extra text around it")
					end
				end
			end

			-- Check the indent level change from the last line, if it's too much, report:
			local indentStart, indentEnd = a_Line:find("\t+")
			local indentLevel = 0
			if (indentStart) then
				indentLevel = indentEnd - indentStart
			end
			if (indentLevel > 0) then
				if ((lastIndentLevel - indentLevel >= 2) or (lastIndentLevel - indentLevel <= -2)) then
					ReportViolation(a_FileName, lineCounter, 1, indentStart or 1, "Indent changed more than a single level between the previous line and this one: from " .. lastIndentLevel .. " to " .. indentLevel)
				end
				lastIndentLevel = indentLevel
			end

			-- Check that control statements have braces on separate lines after them:
			-- Note that if statements can be broken into multiple lines, in which case this test is not taken
			if (isLastLineControl) then
				if not(a_Line:find("^%s*{") or a_Line:find("^%s*#")) then
					-- Not followed by a brace, not followed by a preprocessor
					ReportViolation(a_FileName, lineCounter - 1, 1, 1, "Control statement needs a brace on separate line")
				end
			end
			local lineWithSpace = " " .. a_Line
			isLastLineControl =
				lineWithSpace:find("^%s+if %b()") or
				lineWithSpace:find("^%s+else if %b()") or
				lineWithSpace:find("^%s+for %b()") or
				lineWithSpace:find("^%s+switch %b()") or
				lineWithSpace:find("^%s+else\n") or
				lineWithSpace:find("^%s+else  //") or
				lineWithSpace:find("^%s+do %b()")


			-- Check that exactly 5 empty lines are left beteen functions and no more than 5 elsewhere
			if not(a_Line:find("^\n")) then
				local numEmptyLines = (lineCounter - lastNonEmptyLine) - 1

				local isStartOfFunction = (
					isAfterFunction and
					a_Line:find("^[%s%w]")
				)

				if (isSourceFile and isStartOfFunction and (numEmptyLines ~= 5)) then
					ReportViolation(a_FileName, lineCounter - 1, 1, 1, "Leave exactly 5 empty lines between functions (found " .. numEmptyLines ..")")
				elseif (numEmptyLines > 5) then
					ReportViolation(a_FileName, lineCounter - 1, 1, 1, "Leave at most 5 consecutive empty lines (found " .. numEmptyLines .. ")")
				end

				lastNonEmptyLine = lineCounter
				isAfterFunction = (a_Line == "}\n")
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





--- Array of files to process. Filled from cmdline arguments
local ToProcess = {}





--- Handlers for the command-line arguments
-- Maps flag => function
local CmdLineHandlers =
{
	-- "-f file" checks the specified file
	["-f"] = function (a_Args, a_Idx)
		local fnam = a_Args[a_Idx + 1]
		if not(fnam) then
			error("Invalid flag: '-f' needs a filename following it.")
		end
		table.insert(ToProcess, fnam)
		return a_Idx + 2  -- skip the filename in param parsing
	end,

	-- "-g" checks files reported by git as being committed.
	["-g"] = function (a_Args, a_Idx)
		local f = io.popen("git diff --cached --name-only --diff-filter=ACMR")
		for fnam in f:lines() do
			table.insert(ToProcess, fnam)
		end
	end,

	-- "-h" prints help and exits
	["-h"] = function (a_Args, a_Idx)
		print([[
Usage:")
"CheckBasicStyle [<options>]

Available options:
-f <filename> - checks the specified filename
-g            - checks files reported by Git as being committed
-h            - prints this help and exits
-l <listfile> - checks all files listed in the specified listfile
--            - reads the list of files to check from stdin

When no options are given, the script checks all files listed in the AllFiles.lst file.

Only .cpp and .h files are ever checked.
]])
		os.exit(0)
	end,

	-- "-l listfile" loads the list of files to check from the specified listfile
	["-l"] = function (a_Args, a_Idx)
		local listFile = a_Args[a_Idx + 1]
		if not(listFile) then
			error("Invalid flag: '-l' needs a filename following it.")
		end
		for fnam in io.lines(listFile) do
			table.insert(ToProcess, fnam)
		end
		return a_Idx + 2  -- Skip the listfile in param parsing
	end,

	-- "--" reads the list of files from stdin
	["--"] = function (a_Args, a_Idx)
		for fnam in io.lines() do
			table.insert(ToProcess, fnam)
		end
	end,
}





-- Remove buffering from stdout, so that the output appears immediately in IDEs:
io.stdout:setvbuf("no")

-- Parse the cmdline arguments to see what files to check:
local idx = 1
while (arg[idx]) do
	local handler = CmdLineHandlers[arg[idx]]
	if not(handler) then
		error("Unknown command-line argument #" .. idx .. ": " .. arg[idx])
	end
	idx = handler(arg, idx) or (idx + 1)  -- Call the handler, let it change the next index if it wants
end


-- By default process all files in the AllFiles.lst file (generated by cmake):
if not(arg[1]) then
	for fnam in io.lines("AllFiles.lst") do
		table.insert(ToProcess, fnam)
	end
end





-- Process the files in the list:
for _, fnam in ipairs(ToProcess) do

	-- Remove the optional "./" prefix:
	if (fnam:sub(1, 2) == "./") then
		fnam = fnam:sub(3)
	end

	ProcessItem(fnam)
end





-- Report final verdict:
print("Number of violations found: " .. g_NumViolations)
if (g_NumViolations > 0) then
	os.exit(2)
else
	os.exit(0)
end
