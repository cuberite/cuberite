
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





local access = {public = 0, protected = 1, private = 2}





--- Defines classes that have a custom manual Push() implementation and should not generate the automatic one
-- Map of classname -> true
local g_HasCustomPushImplementation =
{
	cEntity = true
}





function parser_hook(s)
	local container = classContainer.curr -- get the current container

	-- process access-specifying labels (public, private, etc)
	do
		local b, e, label = string.find(s, "^%s*(%w*)%s*:[^:]") -- we need to check for [^:], otherwise it would match 'namespace::type'
		if b then

			-- found a label, get the new access value from the global 'access' table
			if access[label] then
				container.curr_member_access = access[label]
			end -- else ?

			return strsub(s, e) -- normally we would use 'e+1', but we need to preserve the [^:]
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
			if not(g_HasCustomPushImplementation[item.name]) then
				f:write("void Push(" .. item.name .. " * a_Value);\n")
			end
		end
		for _, item in ipairs(types) do
			f:write("bool GetStackValue(int a_StackPos, Ptr" .. item.lname .. " & a_ReturnedVal);\n")
			f:write("bool GetStackValue(int a_StackPos, ConstPtr" .. item.lname .. " & a_ReturnedVal);\n")
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
			if not(g_HasCustomPushImplementation[item.name]) then
				f:write("void cLuaState::Push(" .. item.name .. " * a_Value)\n{\n\tASSERT(IsValid());\n")
				f:write("\ttolua_pushusertype(m_LuaState, a_Value, \"" .. item.name .. "\");\n");
				f:write("\tm_NumCurrentFunctionArgs += 1;\n")
				f:write("}\n\n\n\n\n\n")
			end
		end
		for _, item in ipairs(types) do
			f:write("bool cLuaState::GetStackValue(int a_StackPos, Ptr" .. item.lname .. " & a_ReturnedVal)\n{\n\tASSERT(IsValid());\n")
			f:write("\tif (lua_isnil(m_LuaState, a_StackPos))\n\t{\n")
			f:write("\t\ta_ReturnedVal = nullptr;\n")
			f:write("\t\treturn false;\n\t}\n")
			f:write("\ttolua_Error err;\n")
			f:write("\tif (tolua_isusertype(m_LuaState, a_StackPos, \"" .. item.name .. "\", false, &err))\n")
			f:write("\t{\n")
			f:write("\t\ta_ReturnedVal = *(reinterpret_cast<" .. item.name .. " **>(lua_touserdata(m_LuaState, a_StackPos)));\n")
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
			f:write("\t\ta_ReturnedVal = *(reinterpret_cast<const " .. item.name .. " **>(lua_touserdata(m_LuaState, a_StackPos)));\n")
			f:write("\t\treturn true;\n");
			f:write("\t}\n")
			f:write("\treturn false;\n")
			f:write("}\n\n\n\n\n\n")
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
		a_File:write("\t\t\t", fn.Name, " =\n\t\t\t{\n")
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
			a_File:write("\t\t\t\t\t}\n\t\t\t\t\tDesc = ", string.format("%q", desc.DoxyComment or ""), ",\n")
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
		a_File:write("\t\t\t\tDesc = [[", v.Desc.DoxyComment or "", "]],\n")
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
		a_File:write("\t\t\t\tDesc = [[", con.Desc.DoxyComment or "", "]],\n")
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
		local idx = 1
		for _, valname in ipairs(enum.lnames) do
			valnames[idx] = valname
			idx = idx + 1
		end
		table.sort(valnames)
		for _, valname in ipairs(valnames) do
			a_File:write("\t\t\t\t\"", valname, "\",\n")
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
	f:write("\t\tDesc = [[", a_Class.doxycomment or "", "]],\n")
	
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
	f:write("\t}\n}\n")
	f:close()
end





local function genPackageDocs(a_Self)
	-- Generate docs for each member:
	local i = 1
	local filenames = {}
	while (a_Self[i]) do
		if (a_Self[i].genDocs) then
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
		if (a_Self[i].genMemberDocs) then
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
	
	if (a_Self.csetname) then
		a_Variables[a_Self.lname] = desc
	else
		a_Constants[a_Self.lname] = desc
	end
end





function pre_output_hook(a_Package)
	OutputLuaStateHelpers(a_Package)
end





function post_output_hook(a_Package)
	-- Generate the documentation:
	classPackage.genDocs = genPackageDocs
	classClass.genDocs = genClassDocs
	classFunction.genMemberDocs = genFunctionMemberDocs
	classVariable.genMemberDocs = genVariableMemberDocs
	a_Package:genDocs()
	
	print("Lua bindings and docs have been generated.")
end





if not(TOLUA_VERSION) then
	-- BindingsProcessor has been called standalone, invoke the entire ToLua++ machinery:
	print("Generating Lua bindings and docs...")
	invokeToLua()
	return
end




