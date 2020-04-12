
-- BindingsProcessor.lua

-- Implements additional processing that is done while generating the Lua bindings

--[[
The primary purpose of this file is to provide transformations for ToLua - it is loaded by ToLua++
before processing the C++ code.

This file can also be used as a standalone Lua program to actually generate the bindings, it invokes
ToLua++ if executed by a regular Lua interpreter

The transformations implemented:
	- Modify ToLua++ behavior so that it doesn't generate bindings for private and protected members
	- Export additional files to be included in cLuaState:
		- Forward declarations and typedefs for custom classes' pointers
		- Pushing and popping of bindings' classes

To parse DoxyComments, the preprocessor first replaces them with markers and then the parser uses
those markers to apply the DoxyComment to the next or previous item in the container, based on
the DoxyComment type.

Placeholders in use (i = internal ToLua++):
	- \1 and \2: (i) Embedded Lua code
	- \3 and \4: (i) Embedded C code ("<>")
	- \5 and \6: (i) Embedded C code ("{}")
	- \17 and \18: DoxyComment for next item ("/** ... */") via an ID lookup
	- \19 and \20: DoxyComment for previous item ("///< ... \n") via an ID lookup
--]]





--- Invokes the ToLua++ parser
-- Called when this script detects it has been run outside of ToLua++'s processing
local function invokeToLua()
	-- The values used by ToLua scripts, normally filled from the cmdline params:
	flags =
	{
		L = "BindingsProcessor.lua",
		o = "Bindings.cpp",
		H = "Bindings.h",
		f = "AllToLua.pkg",
		-- P = true,  -- Prints the structure to stdout, doesn't generate cpp file
	}
	_extra_parameters = {}
	TOLUA_VERSION = "tolua++-1.0.92"
	TOLUA_LUA_VERSION = "Lua 5.1"

	-- Path to the ToLua scripts
	path = "../../lib/tolua++/src/bin/lua/"

	-- Run the ToLua processing:
	dofile(path .. "all.lua")
end





local access =
{
	public = 0,
	protected = 1,
	private = 2
}





--- Defines classes that have a custom manual Push() implementation and should not generate the automatic one
-- Map of classname -> true
local g_HasCustomPushImplementation =
{
	cEntity = true,
}





--- Defines classes that have a custom manual GetStackValue() implementation and should not generate the automatic one
-- Map of classname -> true
local g_HasCustomGetImplementation =
{
	Vector3d = true,
	Vector3f = true,
	Vector3i = true,
}





--- Array-table of forward DoxyComments that are replaced in preprocess_hook() and substituted back in parser_hook()
-- We need to use a lookup table because the comments themselves may contain "//" which the preprocessor
-- would eliminate, thus breaking the code
-- The "n" member is a counter for faster insertion
local g_ForwardDoxyComments =
{
	n = 0
}


--- Array-table of backward DoxyComments that are replaced in preprocess_hook() and substituted back in parser_hook()
-- We need to use a lookup table because the comments themselves may contain "//" which the preprocessor
-- would eliminate, thus breaking the code
-- The "n" member is a counter for faster insertion
local g_BackwardDoxyComments =
{
	n = 0,
}





--- Provides extra parsing in addition to ToLua++'s own
-- Called by ToLua++ each time it extracts the next piece of code to parse
-- a_Code is the string representing the code to be parsed
-- The return value is the remaining code to be parsed in the next iteration
-- Processes the class access specifiers (public, protected, private), and doxycomments
function parser_hook(a_Code)
	-- Process access-specifying labels (public, private, etc)
	do
		local b, e, label = string.find(a_Code, "^%s*(%w*)%s*:[^:]") -- we need to check for [^:], otherwise it would match 'namespace::type'
		if b then
			-- Found a label, get the new access value for it:
			if access[label] then
				classContainer.curr.curr_member_access = access[label]
			end -- else ?
			return strsub(a_Code, e) -- normally we would use 'e + 1', but we need to preserve the [^:]
		end
	end

	-- Process forward DoxyComments:
	do
		local b, e, comment = a_Code:find("^%s*(%b\17\18)")
		if (b) then
			local curr = classContainer.curr
			if (curr.n and (curr.n > 0)) then
				curr[curr.n].next_DoxyComment = g_ForwardDoxyComments[tonumber(comment:sub(2, -2))]
			else
				curr.first_child_DoxyComment = g_ForwardDoxyComments[tonumber(comment:sub(2, -2))]
			end
			return strsub(a_Code, e + 1)
		end
	end

	-- Process backward DoxyComments:
	do
		local b, e, comment = a_Code:find("^%s*(%b\19\20)")
		if (b) then
			comment = g_BackwardDoxyComments[tonumber(comment:sub(2, -2))]
			local currContainer = classContainer.curr
			if (currContainer.n > 0) then
				currContainer[currContainer.n].DoxyComment = comment
			else
				print("Backward DoxyComment lost in " .. (currContainer.name or currContainer.lname or currContainer.cname or "<no name>"))
			end
			return strsub(a_Code, e + 1)
		end
	end
end





--- Outputs the helper files supplementing the cLuaState class
-- Writes:
--   LuaState_Declaration.inc
--   LuaState_Implementation.cpp
--   LuaState_Typedefs.inc
local function OutputLuaStateHelpers(a_Package)
	-- Collect all class types from ToLua:
	local types = {}
	for idx, item in ipairs(a_Package) do
		local mt = getmetatable(item) or {}
		if (mt.classtype == "class") then
			table.insert(types, {name = item.name, lname = item.lname})
		end
	end
	table.sort(types,
		function(a_Item1, a_Item2)
			return (a_Item1.name:lower() < a_Item2.name:lower())
		end
	)

	-- Output the typedefs:
	do
		local f = assert(io.open("LuaState_Typedefs.inc", "w"))
		f:write("\n// LuaState_Typedefs.inc\n\n// This file is generated along with the Lua bindings by ToLua. Do not edit manually, do not commit to repo.\n")
		f:write("// Provides a forward declaration and a typedef for a pointer to each class exported to the Lua API.\n")
		f:write("\n\n\n\n\n")
		for _, item in ipairs(types) do
			if not(item.name:match(".*<.*")) then  -- Skip templates altogether
				-- Classes start with a "c", everything else is a struct:
				if (item.name:sub(1, 1) == "c") then
					f:write("class " .. item.name .. ";\n")
				else
					f:write("struct " .. item.name .. ";\n")
				end
			end
		end
		f:write("\n\n\n\n\n")
		for _, item in ipairs(types) do
			f:write("typedef " .. item.name .. " * Ptr" .. item.lname .. ";\n")
			f:write("typedef const " .. item.name .. " * ConstPtr" .. item.lname .. ";\n")
		end
		f:write("\n\n\n\n\n")
		f:close()
	end

	-- Output the Push() and GetStackValue() function declarations:
	do
		local f = assert(io.open("LuaState_Declaration.inc", "w"))
		f:write("\n// LuaState_Declaration.inc\n\n// This file is generated along with the Lua bindings by ToLua. Do not edit manually, do not commit to repo.\n")
		f:write("// Implements a Push() and GetStackValue() function for each class exported to the Lua API.\n")
		f:write("// This file expects to be included form inside the cLuaState class definition\n")
		f:write("\n\n\n\n\n")
		for _, item in ipairs(types) do
			if not(g_HasCustomPushImplementation[item.lname]) then
				f:write("void Push(" .. item.name .. " * a_Value);\n")
			end
		end
		for _, item in ipairs(types) do
			if not(g_HasCustomGetImplementation[item.lname]) then
				f:write("bool GetStackValue(int a_StackPos, Ptr" .. item.lname .. " & a_ReturnedVal);\n")
				f:write("bool GetStackValue(int a_StackPos, ConstPtr" .. item.lname .. " & a_ReturnedVal);\n")
			end
		end
		f:write("\n\n\n\n\n")
		f:close()
	end

	-- Output the Push() and GetStackValue() function implementations:
	do
		local f = assert(io.open("LuaState_Implementation.cpp", "w"))
		f:write("\n// LuaState_Implementation.cpp\n\n// This file is generated along with the Lua bindings by ToLua. Do not edit manually, do not commit to repo.\n")
		f:write("// Implements a Push() and GetStackValue() function for each class exported to the Lua API.\n")
		f:write("// This file expects to be compiled as a separate translation unit\n")
		f:write("\n\n\n\n\n")
		f:write("#include \"Globals.h\"\n#include \"LuaState.h\"\n#include \"tolua++/include/tolua++.h\"\n")
		f:write("\n\n\n\n\n")
		for _, item in ipairs(types) do
			if not(g_HasCustomPushImplementation[item.lname]) then
				f:write("void cLuaState::Push(" .. item.name .. " * a_Value)\n{\n\tASSERT(IsValid());\n")
				f:write("\ttolua_pushusertype(m_LuaState, a_Value, \"" .. item.name .. "\");\n");
				f:write("}\n\n\n\n\n\n")
			end
		end
		for _, item in ipairs(types) do
			if not(g_HasCustomGetImplementation[item.lname]) then
				f:write("bool cLuaState::GetStackValue(int a_StackPos, Ptr" .. item.lname .. " & a_ReturnedVal)\n{\n\tASSERT(IsValid());\n")
				f:write("\tif (lua_isnil(m_LuaState, a_StackPos))\n\t{\n")
				f:write("\t\ta_ReturnedVal = nullptr;\n")
				f:write("\t\treturn false;\n\t}\n")
				f:write("\ttolua_Error err;\n")
				f:write("\tif (tolua_isusertype(m_LuaState, a_StackPos, \"" .. item.name .. "\", false, &err))\n")
				f:write("\t{\n")
				f:write("\t\ta_ReturnedVal = *(static_cast<" .. item.name .. " **>(lua_touserdata(m_LuaState, a_StackPos)));\n")
				f:write("\t\treturn true;\n");
				f:write("\t}\n")
				f:write("\treturn false;\n")
				f:write("}\n\n\n\n\n\n")

				f:write("bool cLuaState::GetStackValue(int a_StackPos, ConstPtr" .. item.lname .. " & a_ReturnedVal)\n{\n\tASSERT(IsValid());\n")
				f:write("\tif (lua_isnil(m_LuaState, a_StackPos))\n\t{\n")
				f:write("\t\ta_ReturnedVal = nullptr;\n")
				f:write("\t\treturn false;\n\t}\n")
				f:write("\ttolua_Error err;\n")
				f:write("\tif (tolua_isusertype(m_LuaState, a_StackPos, \"const " .. item.name .. "\", false, &err))\n")
				f:write("\t{\n")
				f:write("\t\ta_ReturnedVal = *(static_cast<const " .. item.name .. " **>(lua_touserdata(m_LuaState, a_StackPos)));\n")
				f:write("\t\treturn true;\n");
				f:write("\t}\n")
				f:write("\treturn false;\n")
				f:write("}\n\n\n\n\n\n")
			end
		end
		f:close()
	end
end





local function FormatString(a_Str)
	local fmt = string.format("%q", a_Str)
	return (string.gsub(string.gsub(fmt, "\\\n", "\\n"), "\\\r\n", "\\r\\n"))
end





local function OutputTable(a_File, a_Table, a_Name, a_Indent, a_Visited, a_Metas)
	-- Check and update the "visited" status:
	if (a_Visited[a_Table]) then
		a_File:write(a_Indent .. "{ \"visited: " .. a_Visited[a_Table] .. "\", }")
		return
	end
	a_Visited[a_Table] = a_Name

	-- Output the table contents:
	a_File:write(a_Indent .. "{\n")
	local indent = a_Indent .. "\t"
	for k, v in pairs(a_Table) do
		if (type(k) == "string") then
			a_File:write(indent .. "[" .. FormatString(k) .. "] =")
		else
			a_File:write(indent .. "[" .. tostring(k) .. "] =")
		end
		local t = type(v)
		if (
			(t == "number") or
			(t == "boolean")
		) then
			a_File:write(" ", tostring(v))
		elseif (t == "string") then
			a_File:write(" ", FormatString(v))
		elseif (t == "table") then
			local metatab = getmetatable(v)
			if (metatab) then
				a_File:write("  -- meta: " .. tostring(metatab))
				a_Metas[metatab] = metatab
			end
			a_File:write("\n")
			OutputTable(a_File, v, a_Name .. "." .. tostring(k), indent, a_Visited, a_Metas)
		else
			print("Unhandled type: " .. t .. ": " .. tostring(v))
			a_File:write(" ", tostring(v))
		end
		a_File:write(",\n")
	end  -- for k, v - a_Table
	a_File:write(a_Indent .. "}")
end





--- Outputs the docs for all the functions in the specified class
-- a_File is the output file
-- a_Class is the ToLua's classClass object
-- a_Functions is a dictionary of function descriptions: "name" -> { {<description>}, ...}
local function outputClassFunctionDocs(a_File, a_Class, a_Functions)
	-- Sort the functions by name:
	local functions = {}
	for name, descs in pairs(a_Functions) do
		table.insert(functions, { Name = name, Descs = descs })
	end
	table.sort(functions,
		function (a_Fn1, a_Fn2)
			return (a_Fn1.Name < a_Fn2.Name)
		end
	)

	-- If there are no functions, bail out:
	if not(functions[1]) then
		return
	end

	-- Output the descriptions:
	a_File:write("\t\tFunctions =\n\t\t{\n")
	for _, fn in ipairs(functions) do
		local name = fn.Name
		if (name:sub(1, 1) == ".") then
			name = "[\"" .. name .. "\"]"
		end
		a_File:write("\t\t\t", name, " =\n\t\t\t{\n")
		for _, desc in ipairs(fn.Descs) do
			a_File:write("\t\t\t\t{\n\t\t\t\t\tParams =\n\t\t\t\t\t{\n")
			for _, param in ipairs(desc.Parameters) do
				a_File:write("\t\t\t\t\t\t{\n")
				a_File:write("\t\t\t\t\t\t\tType = \"", param.Type, "\",\n")
				a_File:write("\t\t\t\t\t\t\tName = \"", param.Name, "\",\n")
				a_File:write("\t\t\t\t\t\t},\n")
			end
			a_File:write("\t\t\t\t\t},\n\t\t\t\t\tReturns =\n\t\t\t\t\t{\n")
			for _, ret in ipairs(desc.Returns) do
				a_File:write("\t\t\t\t\t\t{\n\t\t\t\t\t\t\tType = \"", ret.Type, "\",\n\t\t\t\t\t\t},\n")
			end
			a_File:write("\t\t\t\t\t},\n")
			if (desc.IsStatic) then
				a_File:write("\t\t\t\t\tIsStatic = true,\n")
			end
			if (desc.DoxyComment) then
				a_File:write("\t\t\t\t\tDesc = ", string.format("%q", desc.DoxyComment), ",\n")
			end
			a_File:write("\t\t\t\t},\n")
		end
		a_File:write("\t\t\t},\n")
	end
	a_File:write("\t\t},\n")
end





--- Outputs the docs for all the member variables in the specified class
-- a_File is the output file
-- a_Class is the ToLua's classClass object
-- a_Variables is a dictionary of variable descriptions: "name" -> {<description>}
local function outputClassVariableDocs(a_File, a_Class, a_Variables)
	-- Sort the variables by name:
	local variables = {}
	for name, desc in pairs(a_Variables) do
		table.insert(variables, { Name = name, Desc = desc })
	end
	table.sort(variables,
		function (a_Var1, a_Var2)
			return (a_Var1.Name < a_Var2.Name)
		end
	)

	-- If there are no variables, bail out:
	if not(variables[1]) then
		return
	end

	-- Output the descriptions:
	a_File:write("\t\tVariables =\n\t\t{\n")
	for _, v in ipairs(variables) do
		a_File:write("\t\t\t", v.Name, " =\n\t\t\t{\n")
		a_File:write("\t\t\t\tType = \"", v.Desc.Type, "\",\n")
		if (v.Desc.DoxyComment) then
			a_File:write("\t\t\t\tDesc = ", string.format("%q", v.Desc.DoxyComment), ",\n")
		end
		a_File:write("\t\t\t},\n")
	end
	a_File:write("\t\t},\n")
end





--- Outputs the docs for all the member constants in the specified class
-- a_File is the output file
-- a_Class is the ToLua's classClass object
-- a_Constants is a dictionary of constant descriptions: "name" -> {<description>}
-- a_IgnoredConstants is a dictionary of constants not to be exported: "name" -> true (used for ToLua++'s multi-inheritance)
local function outputClassConstantDocs(a_File, a_Class, a_Constants, a_IgnoredConstants)
	-- Sort the constants by name:
	local constants = {}
	for name, desc in pairs(a_Constants) do
		if not(a_IgnoredConstants[name]) then
			table.insert(constants, { Name = name, Desc = desc })
		end
	end
	table.sort(constants,
		function (a_Var1, a_Var2)
			return (a_Var1.Name < a_Var2.Name)
		end
	)

	-- If there are no constants, bail out:
	if not(constants[1]) then
		return
	end

	-- Output the descriptions:
	a_File:write("\t\tConstants =\n\t\t{\n")
	for _, con in ipairs(constants) do
		a_File:write("\t\t\t", con.Name, " =\n\t\t\t{\n")
		a_File:write("\t\t\t\tType = \"", con.Desc.Type, "\",\n")
		if (con.Desc.DoxyComment) then
			a_File:write("\t\t\t\tDesc = ", string.format("%q", con.Desc.DoxyComment), ",\n")
		end
		a_File:write("\t\t\t},\n")
	end
	a_File:write("\t\t},\n")
end





--- Outputs the docs for all the member enums in the specified class
-- a_File is the output file
-- a_Class is the ToLua's classClass object
-- a_Enums is an array of ToLua's classEnum objects
local function outputClassEnumDocs(a_File, a_Class, a_Enums)
	-- If there are no enums, bail out:
	if (not(a_Enums) or not(a_Enums[1])) then
		return
	end

	-- Sort the enums by name:
	table.sort(a_Enums,
		function (a_Enum1, a_Enum2)
			return (a_Enum1.name < a_Enum2.name)
		end
	)

	-- Output the enums:
	a_File:write("\t\tEnums =\n\t\t{\n")
	for i, enum in ipairs(a_Enums) do
		local name = enum.name
		if (not(name) or (name == "")) then
			name = string.format("unnamedEnum_%d", i)
		end
		a_File:write("\t\t\t", name, " =\n\t\t\t{\n")
		local valnames = {}
		-- Make a copy of enum.lnames so that we can sort it:
		local idx = 1
		for i, valname in ipairs(enum.lnames) do
			valnames[idx] = { Name = valname, DoxyComment = enum.DoxyComments[i] }
			idx = idx + 1
		end
		table.sort(valnames,
			function (a_Val1, a_Val2)
				return (a_Val1.Name < a_Val2.Name)
			end
		)
		for _, valname in ipairs(valnames) do
			assert(not(valname.Name:find("\17")))
			assert(not(valname.Name:find("\18")))
			assert(not(valname.Name:find("\19")))
			assert(not(valname.Name:find("\20")))
			a_File:write("\t\t\t\t{\n")
			a_File:write("\t\t\t\t\tName = \"", valname.Name, "\",\n")
			if (valname.DoxyComment) then
				a_File:write("\t\t\t\t\tDesc = ", string.format("%q", valname.DoxyComment), ",\n")
			end
			a_File:write("\t\t\t\t},\n")
		end
		a_File:write("\t\t\t},\n")
	end
	a_File:write("\t\t},\n")
end





--- Outputs the docs for the specified class, which has been parsed for its functions, variables and constants
-- a_Class is the ToLua's classClass object
-- a_Functions is a dictionary of function descriptions: "name" -> { {<description>}, ...}
-- a_Variables is a dictionary of variable descriptions: "name" -> {<description>}
-- a_Constants is a dictionary of constant descriptions: "name" -> {<description>}
-- a_Filenames is an array into which the name of the docs file is to be appended
local function outputClassDocs(a_Class, a_Functions, a_Variables, a_Constants, a_Filenames)
	-- Add the output file to list of filenames:
	local fnam = a_Class.lname .. ".lua"
	table.insert(a_Filenames, fnam)

	-- Output the header:
	local f = assert(io.open("docs/" .. fnam, "w"))
	f:write("return\n{\n\t", a_Class.lname, " =\n\t{\n")
	if (a_Class.DoxyComment) then
		f:write("\t\tDesc = ", string.format("%q", a_Class.DoxyComment), ",\n")
	end

	-- If the class inherits from anything, output it here:
	local ignoredConstants = {}
	if (a_Class.base and (a_Class.base ~= "")) then
		local bases = {a_Class.base}
		local idx = 2
		for _, b in ipairs(a_Class.extra_bases or {}) do
			bases[idx] = b
			idx = idx + 1
			-- ToLua++ handles multiple inheritance by adding "constants" for the base types; ignore those:
			ignoredConstants["__" .. b .. "__"] = true
		end
		table.sort(bases)
		f:write("\t\tInherits =\n\t\t{\n")
		for _, b in ipairs(bases) do
			f:write("\t\t\t", string.format("%q", b), ",\n")
		end
		f:write("\t\t},\n")
	end

	-- Output the functions:
	outputClassFunctionDocs(f, a_Class, a_Functions)

	-- Output the variables:
	outputClassVariableDocs(f, a_Class, a_Variables)

	-- Output the constants:
	outputClassConstantDocs(f, a_Class, a_Constants, ignoredConstants)

	-- Output the enums:
	outputClassEnumDocs(f, a_Class, a_Class.enums)

	-- Output the footer:
	f:write("\t},\n}\n")
	f:close()
end





--- Recursively applies the next_DoxyComment member to the next item, and first_child_DoxyComment to first child item in the container.
-- a_Container is the ToLua++'s table potentially containing children as its array members
local function applyNextDoxyComments(a_Container)
	-- Apply the DoxyComment to the first child, if appropriate:
	if (a_Container[1] and a_Container.first_child_DoxyComment) then
		a_Container[1].DoxyComment = a_Container.first_child_DoxyComment
	end

	-- Apply each child's next_DoxyComment to the actual next child:
	local i = 1
	while (a_Container[i]) do
		if (a_Container[i].next_DoxyComment) then
			if (a_Container[i + 1]) then
				a_Container[i + 1].DoxyComment = a_Container[i].next_DoxyComment
			end
		end
		applyNextDoxyComments(a_Container[i])
		i = i + 1
	end
end





--- Generates documentation for a package.
local function genPackageDocs(a_Self)
	-- DEBUG: Output the raw package object:
	do
		local f = io.open("docs/_raw.lua", "w")
		if (f) then
			OutputTable(f, a_Self, "", "", {}, {})
			f:close()
		end
	end

	applyNextDoxyComments(a_Self)

	-- Generate docs for each member:
	local i = 1
	local filenames = {}
	while (a_Self[i]) do
		if (
			a_Self[i]:check_public_access() and  -- Do not export private and protected members
			a_Self[i].genDocs
		) then
			a_Self[i]:genDocs(filenames)
		end
		i = i + 1
	end

	-- Output the globals' docs:
	local functions = {}
	local variables = {}
	local constants = {}
	while (a_Self[i]) do
		if (a_Self[i].genMemberDocs) then
			a_Self[i]:genMemberDocs(functions, variables, constants)
		end
		i = i + 1
	end
	local oldName = a_Self.lname
	a_Self.lname = "Globals"
	outputClassDocs(a_Self, functions, variables, constants, filenames)
	a_Self.lname = oldName

	-- Output the list of docs files:
	table.sort(filenames)
	local f = assert(io.open("docs/_files.lua", "w"))
	f:write("return\n{\n")
	for _, fnam in ipairs(filenames) do
		f:write("\t\"", fnam, "\",\n")
	end
	f:write("}\n")
	f:close()
end





local function genClassDocs(a_Self, a_Filenames)
	assert(a_Self.lname)
	assert(type(a_Filenames) == "table")
	--[[
	print("\n\nGenerating docs for class " .. a_Self.lname)
	local visited = {[a_Self.parent] = "<package>"}
	local metas = {}
	OutputTable(io.stdout, a_Self, a_Self.lname, "", visited, metas)
	--]]

	-- Collect the function, variable and constant docs:
	local i = 1
	local functions = {}
	local variables = {}
	local constants = {}
	while (a_Self[i]) do
		if (
			a_Self[i]:check_public_access() and  -- Don't export private and protected members
			a_Self[i].genMemberDocs
		) then
			a_Self[i]:genMemberDocs(functions, variables, constants)
		end
		i = i + 1
	end

	-- Output the individual docs
	outputClassDocs(a_Self, functions, variables, constants, a_Filenames)
end





--- Parses the specified function's parameters and returns their description as a table
-- a_Function is the ToLua's classFunction object
local function parseFunctionParameters(a_Function)
	-- If the only param is a "void", then report no params:
	if (
		a_Function.args and                  -- The params are present
		(#(a_Function.args) == 1) and        -- There is exactly one param
		(a_Function.args[1].type == "void")  -- The param is a void
	) then
		return {}
	end

	local res = {}
	local idx = 1
	for _, param in ipairs(a_Function.args or {}) do
		local t = param.type
		t = t:gsub("^const ", "")  -- Remove the "const" keyword, if present
		res[idx] =
		{
			Name = param.name,
			Type = t,
			IsConst = (param.type:match("^const ") ~= nil),
		}
		idx = idx + 1
	end
	return res
end





--- Parses the specified function's return values and returns their description as a table
-- a_Function is the ToLua's classFunction object
local function parseFunctionReturns(a_Function)
	local res = {}
	local idx = 1
	if (a_Function.type and (a_Function.type ~= "void")) then
		res[idx] = { Type = a_Function.type }
		idx = idx + 1
	end
	for _, param in ipairs(a_Function.args or {}) do
		if ((param.mod == "&") or (param.ret == "&")) then
			res[idx] = { Type = param.type:gsub("^const ", "") }
			idx = idx + 1
		end
	end
	return res
end





local function genFunctionMemberDocs(a_Self, a_Functions, a_Variables, a_Constants)
	assert(a_Self.lname)

	local fn = a_Functions[a_Self.lname] or {}
	a_Functions[a_Self.lname] = fn

	local desc =
	{
		LuaName = a_Self.lname,
		CType = a_Self.type,
		DoxyComment = a_Self.DoxyComment,
		Parameters = parseFunctionParameters(a_Self),
		Returns = parseFunctionReturns(a_Self),
	}
	local _, _, hasStatic = string.find(a_Self.mod, "^%s*(static)")
	if (hasStatic) then
		desc.IsStatic = true
	end
	table.insert(fn, desc)
end





local function genVariableMemberDocs(a_Self, a_Functions, a_Variables, a_Constants)
	assert(a_Self.lname)

	local desc =
	{
		Name = a_Self.lname,
		Type = a_Self.type,
		DoxyComment = a_Self.DoxyComment,
	}

	if (string.find(a_Self.type,'const%s+') or string.find(a_Self.mod, 'tolua_readonly') or string.find(a_Self.mod, 'tolua_inherits'))  then
		a_Constants[a_Self.lname] = desc
	else
		a_Variables[a_Self.lname] = desc
	end
end





--- Generates the entire documentation for the API
-- a_Package is ToLua++'s classPackage object
-- Checks if the documentation folder is writable, if not, skips the docs generation
-- Returns true if documentation was generated, false and message if not
local function generateDocs(a_Package)
	-- Check if the docs folder is writable:
	local f, msg = io.open("docs/_files.lua", "w")
	if not(f) then
		return false, "Cannot access the docs folder: " .. msg
	end
	f:close()

	-- Generate the docs:
	classPackage.genDocs = genPackageDocs
	classClass.genDocs = genClassDocs
	classFunction.genMemberDocs = genFunctionMemberDocs
	classVariable.genMemberDocs = genVariableMemberDocs
	a_Package:genDocs()
	return true
end





--- Outputs the cLuaState helper files.
-- Called by ToLua++ before it starts outputting its generated files.
-- a_Package is ToLua++'s classPackage object
function pre_output_hook(a_Package)
	OutputLuaStateHelpers(a_Package)

	-- Generate the documentation:
	-- (must generate documentation before ToLua++ writes the bindings, because "static" information is lost at that point)
	local isSuccess, msg = generateDocs(a_Package)
	if not(isSuccess) then
		print("API docs haven't been generated due to an error: " .. (msg or "<no message>"))
	else
		print("API docs have been generated.");
	end
end





--- Outputs the documentation files.
-- Called by ToLua++ after writing its generated files.
-- a_Package is ToLua++'s classPackage object
function post_output_hook(a_Package)
	print("Lua bindings have been generated.")
end





--- Provides DoxyComment processing while parsing the C++ code.
-- Called by ToLua++ parser before it starts parsing the code.
-- a_Package is the ToLua++'s classPackage object, currently unparsed
-- The C++ code to be parsed is in a_Packade.code
function preprocess_hook(a_Package)
	assert(a_Package)
	assert(type(a_Package.code) == "string")

	-- Replace all DoxyComments with placeholders so that they aren't erased later on:
	a_Package.code = a_Package.code
		:gsub("/%*%*%s*(.-)%s*%*/",
			function (a_Comment)
				local n = g_ForwardDoxyComments.n + 1
				g_ForwardDoxyComments[n] = a_Comment
				g_ForwardDoxyComments.n = n
				return "\17" .. n .."\18"
			end
		)  -- Replace /** ... */ with an ID into a lookup table wrapped in DC1 and DC2
		:gsub("///<%s*(.-)%s*\n%s*",
			function (a_Comment)
				local n = g_BackwardDoxyComments.n + 1
				g_BackwardDoxyComments[n] = a_Comment
				g_BackwardDoxyComments.n = n
				return "\19" .. n .."\20\n"
			end
		)  -- Replace ///< comments with an ID into a lookup table wrapped in DC3 and DC4

	--[[
	-- Output the preprocessed code out to a file for manual inspection:
	local f = io.open("code_out.cpp", "wb")
	f:write(a_Package.code)
	f:close()
	--]]
end





--- Chooses the smaller of the indices, and the number indicating whether it chose the first or the second
-- If one of the indices is nil, returns the other one
-- If both indices are nil, returns nil
local function chooseNextIndex(a_Index1, a_Index2)
	if not(a_Index1) then
		return a_Index2, 2
	end
	if not(a_Index2) then
		return a_Index1, 1
	end
	if (a_Index1 > a_Index2) then
		return a_Index2, 2
	else
		return a_Index1, 1
	end
end





--- Override for ToLua++'s own code extraction
-- Called for each "$cfile" and "$hfile" directive in the package file
-- a_FileName is the C++ header filename
-- a_Contents is the code contents of the header file
-- The function returns the code to be parsed by ToLua++
-- In addition to the original function, this override extracts all DoxyComments as well
-- This is needed when a function is marked with "// tolua_export" but its DoxyComment is not included
function extract_code(a_FileName, a_Contents)
	local code = '\n$#include "' .. a_FileName .. '"\n'
	a_Contents= "\n" .. a_Contents .. "\n" -- add blank lines as sentinels
	local _, e, c, t = strfind(a_Contents, "\n([^\n]-)[Tt][Oo][Ll][Uu][Aa]_([^%s]*)[^\n]*\n")
	local dcb, dce, dc = strfind(a_Contents, "/%*%*.-%*/")
	local nextEnd, whichOne = chooseNextIndex(e, dce)
	while (nextEnd) do
		if (whichOne == 2) then
			code = code .. a_Contents:sub(dcb, dce) .. "\n"
		else
			t = strlower(t)
			if (t == "begin") then
				_, nextEnd, c = strfind(a_Contents,"(.-)\n[^\n]*[Tt][Oo][Ll][Uu][Aa]_[Ee][Nn][Dd][^\n]*\n", e)
				if not(nextEnd) then
				 tolua_error("Unbalanced 'tolua_begin' directive in header file " .. a_FileName)
				end
			end
			code = code .. c .. "\n"
		end
		_, e, c, t = strfind(a_Contents, "\n([^\n]-)[Tt][Oo][Ll][Uu][Aa]_([^%s]*)[^\n]*\n", nextEnd)
		dcb, dce, dc = strfind(a_Contents, "/%*%*.-%*/", nextEnd)
		nextEnd, whichOne = chooseNextIndex(e, dce)
	end
	return code
end





--- Installs a hook that is called by ToLua++ for each instantiation of classEnumerate
-- The hook is used to fix DoxyComments in enums
local function installEnumHook()
	--Hook for normal enums
	local oldEnumerate = Enumerate
	Enumerate = function (a_Name, a_Body, a_VarName, a_Type)
		-- We need to remove the DoxyComment items from the enum
		-- otherwise ToLua++ parser would make an enum value out of them
		a_Body = string.gsub(a_Body, ",[%s\n]*}", "\n}") -- eliminate last ','
		local t = split(strsub(a_Body, 2, -2), ',') -- eliminate braces
		local doxyComments = {}
		local enumValues = {}
		local numEnumValues = 0
		for _, txt in ipairs(t) do
			txt = txt:gsub("(%b\17\18)",
				function (a_CommentID)
					doxyComments[numEnumValues + 1] = g_ForwardDoxyComments[tonumber(a_CommentID:sub(2, -2))]
					return ""
				end
			):gsub("(%b\19\20)",
				function (a_CommentID)
					doxyComments[numEnumValues] = g_BackwardDoxyComments[tonumber(a_CommentID:sub(2, -2))]
					return ""
				end
			)
			if (txt ~= "") then
				numEnumValues = numEnumValues + 1
				enumValues[numEnumValues] = txt
			end
		end
		local res = oldEnumerate(a_Name, "{" .. table.concat(enumValues, ",") .. "}", a_VarName, a_Type)
		res.DoxyComments = doxyComments
		return res
	end

	--Hook for scoped enums
	local oldScopedEnum = ScopedEnum
	ScopedEnum = function (a_Name, a_Body, a_VarName, a_Type)
		-- We need to remove the DoxyComment items from the enum class
		-- otherwise ToLua++ parser would make an enum value out of them
		a_Body = string.gsub(a_Body, ",[%s\n]*}", "\n}") -- eliminate last ','
		local t = split(strsub(a_Body, 2, -2), ',') -- eliminate braces
		local doxyComments = {}
		local enumValues = {}
		local numEnumValues = 0
		for _, txt in ipairs(t) do
			txt = txt:gsub("(%b\17\18)",
				function (a_CommentID)
					doxyComments[numEnumValues + 1] = g_ForwardDoxyComments[tonumber(a_CommentID:sub(2, -2))]
					return ""
				end
			):gsub("(%b\19\20)",
				function (a_CommentID)
					doxyComments[numEnumValues] = g_BackwardDoxyComments[tonumber(a_CommentID:sub(2, -2))]
					return ""
				end
			)
			if (txt ~= "") then
				numEnumValues = numEnumValues + 1
				enumValues[numEnumValues] = txt
			end
		end
		local res = oldScopedEnum(a_Name, "{" .. table.concat(enumValues, ",") .. "}", a_VarName, a_Type)
		res.DoxyComments = doxyComments
		return res
	end
end





if not(TOLUA_VERSION) then
	-- BindingsProcessor has been called standalone, invoke the entire ToLua++ machinery:
	print("Generating Lua bindings and docs...")
	invokeToLua()
	return
else
	-- We're being executed from inside the ToLua++ parser. Install the needed hooks:
	installEnumHook()
end




