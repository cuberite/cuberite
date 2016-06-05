
-- BindingsProcessor.lua

-- Implements additional processing that is done while generating the Lua bindings





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





function pre_output_hook(a_Package)
	OutputLuaStateHelpers(a_Package)
end





function post_output_hook()
	print("Bindings have been generated.")
end




