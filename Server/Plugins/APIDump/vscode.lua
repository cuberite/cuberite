-- vscode.lua

-- Implements the code for exporting definition files which can be used by a Lua-Language-Server





--- Cleans up the name of a parameter so it can be used in a definition file
--- Removes anything containing brackets and removes dashes and spaces.
local function CleanupParameterName(paramName)
	paramName = paramName:gsub("[%- ]", "")
	:gsub("<.->.-</.->", '');
	return paramName
end





--- Cleans up a description so it can be used in a definition file.
--- Uses the standard cleanup function but also removes any newlines.
local function CleanUpDescriptionVSCode(a_Desc)
	return CleanUpDescription(a_Desc)
	:gsub("\n", " ")
end





--- Writes a list of methods into the specified file in VSCode format
local function WriteVSCodeMethods(f, a_NameSpace, a_Methods)
	for _, func in ipairs(a_Methods or {}) do
		f:write("\n---\n")
		f:write("---", CleanUpDescriptionVSCode(func.Notes or ""), "\n");
		f:write("---\n");
		local parameterList = {}
		if (func.Params) then
			local paramNr = 0;
			for _, param in ipairs(func.Params) do
				paramNr = paramNr + 1;
				local paramName = CleanupParameterName(param.Name or ("param" .. paramNr));
				if (paramName:find("%.%.%.")) then
					paramName = "..."
				end
				table.insert(parameterList, paramName)
				if (param.IsOptional and paramName ~= "...") then
					paramName = paramName .. "?"
				end
				
				local paramType = param.Type;
				if (paramType:find("%#")) then
					paramType = paramType:match("%#(.+)");
				end
				f:write("---@param ", paramName, " ", paramType, "\n");
			end
			f:write("---\n");
		end
		
		if (func.Returns) then
			for _, ret in ipairs(func.Returns) do
				f:write("---@return ", ret.Type, "\n");
			end
			f:write("---\n");
		end
		local name = func.Name:find("constructor") and "__call" or func.Name;
		name = name:find("operator") and "__meta" or name
		local parameters = table.concat(parameterList, ", ");
		f:write("function ")
		if (a_NameSpace) then
			f:write(a_NameSpace, ":")
		end
		f:write(name, "(", parameters, ") end\n\n");
	end
end





--- Writes the list of constants. If the value is an enum the value is set from that enum.
--- This is a bit of a hack because Cuberite exports allot of enums as a constant inside 
--- a class or global but documents them as if they are in their own table.
local function WriteVSCodeConstants(f, a_NameSpace, a_Constants, a_Enum)
	if (not a_Constants) then
		return;
	end

	local prefix = ""
	if (a_NameSpace) then
		prefix = a_NameSpace .. ".";
	end
	for _, const in pairs(a_Constants) do
		f:write(prefix)
		if (a_Enum) then
			f:write(const.Name, " = ", prefix, a_Enum, ".", const.Name, "\n")
		else
			local constValue = tostring(const.Value):match("[%w%d]+") or "nil";
			f:write(const.Name, " = ", constValue, "\n")
		end
	end
end





--- Writes a list of constants into the specified file in VSCode format
local function WriteVSCodeEnums(f, a_NameSpace, a_ConstantGroups)
	if (not a_ConstantGroups) then
		return;
	end

	local prefix = "";
	if (a_NameSpace) then
		prefix = a_NameSpace .. "."
	end
	for _, group in pairs(a_ConstantGroups) do
		f:write("---@enum ", group.Name, "\n");
		f:write(prefix, group.Name, " = {\n")
		for _, const in pairs(group.Constants) do
			local constValue = tostring(const.Value):match("[%w%d]+") or "nil";
			f:write("\t", const.Name, " = ", constValue, ",\n")
		end
		f:write("}\n")
		WriteVSCodeConstants(f, a_NameSpace, group.Constants, group.Name);
	end
end





--- Writes all the fields which a class has.
---@param f file*
---@param a_Variables table
local function WriteVSCodeVariables(f, a_Variables)
	for _, variable in ipairs(a_Variables or {}) do
		f:write("---@field ", variable.Name)
		if (variable.Type) then
			local type = variable.Type:match("%w+")
			f:write(" ", type)
		end
		if (variable.Notes) then
			f:write(" ", variable.Notes)
		end
		f:write("\n");
	end
end





--- Writes one Cuberite class definition into the specified file in VSCode format
local function WriteVSCodeClass(a_Class)
	assert(type(a_Class) == "table")

	local f = io.open("vscode/cuberite/library/" .. a_Class.Name .. ".lua", "w");
	f:write("---@meta\n");
	f:write("\n\n---\n---The ", a_Class.Name, " namespace\n");

	local inherit = "";
	if (a_Class.Inherits) then
		inherit = ": " .. a_Class.Inherits.Name 
	end
	f:write("---@class ", a_Class.Name, inherit, "\n");
	WriteVSCodeVariables(f, a_Class.Variables);
	for _, func in pairs(a_Class.Functions or {}) do
		if (func.Name:find("constructor")) then
			local parameters = {};
			for _, param in ipairs(func.Parameters or {}) do
				table.insert(parameters, param.Type);
			end
			f:write("---@operator call(", table.concat(parameters, ","), "):" .. a_Class.Name, "\n")
		end
	end
	f:write("", a_Class.Name, " = {}\n");

	-- Export methods and constants:
	WriteVSCodeEnums(f, a_Class.Name, a_Class.ConstantGroups);
	WriteVSCodeConstants(f, a_Class.Name, a_Class.Constants);
	WriteVSCodeMethods(f, a_Class.Name, a_Class.Functions);

	f:close();
end





--- Dumps the entire API table into a file in the VSCode format
function DumpAPIVSCode(a_API)
	LOG("Dumping VSCode API description...")
	cFile:CreateFolderRecursive("vscode/cuberite/library");

	-- Export each class except Globals, store those aside:
	local Globals
	for _, cls in ipairs(a_API) do
		if (cls.Name ~= "Globals") then
			WriteVSCodeClass(cls)
		else
			Globals = cls
		end
	end

	-- Export the globals:
	if (Globals) then
		local f = io.open("vscode/cuberite/library/Globals.lua", "w");
		f:write("---@meta\n\n");
		WriteVSCodeMethods(f, nil, Globals.Functions)
		WriteVSCodeEnums(f, nil, Globals.ConstantGroups)
		f:close();
	end

	-- Finish the file:
	LOG("VSCode API dumped...")
end




