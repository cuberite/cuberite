
-- main.lua

-- Implements the plugin entrypoint (in this case the entire plugin)





-- Global variables:
g_Plugin = nil;






function Initialize(Plugin)
	g_Plugin = Plugin;
	
	Plugin:SetName("APIDump");
	Plugin:SetVersion(1);
	
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())

	-- dump all available API functions and objects:
	-- DumpAPITxt();
	
	-- Dump all available API object in HTML format into a subfolder:
	DumpAPIHtml();
	
	return true
end






function DumpAPITxt()
	LOG("Dumping all available functions to API.txt...");
	function dump (prefix, a, Output)
		for i, v in pairs (a) do
			if (type(v) == "table") then
				if (GetChar(i, 1) ~= ".") then
					if (v == _G) then
						-- LOG(prefix .. i .. " == _G, CYCLE, ignoring");
					elseif (v == _G.package) then
						-- LOG(prefix .. i .. " == _G.package, ignoring");
					else
						dump(prefix .. i .. ".", v, Output)
					end
				end
			elseif (type(v) == "function") then
				if (string.sub(i, 1, 2) ~= "__") then
					table.insert(Output, prefix .. i .. "()");
				end
			end
		end
	end

	local Output = {};
	dump("", _G, Output);

	table.sort(Output);
	local f = io.open("API.txt", "w");
	for i, n in ipairs(Output) do
		f:write(n, "\n");
	end
	f:close();
	LOG("API.txt written.");
end





function CreateAPITables()
	--[[
	We want an API table of the following shape:
	local API = {
		{
			Name = "cCuboid",
			Functions = {
				{Name = "Sort"},
				{Name = "IsInside"}
			},
			Constants = {
			}
			Descendants = {},  -- Will be filled by ReadDescriptions(), array of class APIs (references to other member in the tree)
		}},
		{
			Name = "cBlockArea",
			Functions = {
				{Name = "Clear"},
				{Name = "CopyFrom"},
				...
			}
			Constants = {
				{Name = "baTypes", Value = 0},
				{Name = "baMetas", Value = 1},
				...
			}
			...
		}}
	};
	local Globals = {
		Functions = {
			...
		},
		Constants = {
			...
		}
	};
	--]]

	local Globals = {Functions = {}, Constants = {}, Descendants = {}};
	local API = {};
	
	local function Add(a_APIContainer, a_ObjName, a_ObjValue)
		if (type(a_ObjValue) == "function") then
			table.insert(a_APIContainer.Functions, {Name = a_ObjName});
		elseif (
			(type(a_ObjValue) == "number") or
			(type(a_ObjValue) == "string")
		) then
			table.insert(a_APIContainer.Constants, {Name = a_ObjName, Value = a_ObjValue});
		end
	end
	
	local function ParseClass(a_ClassName, a_ClassObj)
		local res = {Name = a_ClassName, Functions = {}, Constants = {}, Descendants = {}};
		for i, v in pairs(a_ClassObj) do
			Add(res, i, v);
		end
		return res;
	end
	
	for i, v in pairs(_G) do
		if (
			(v ~= _G) and           -- don't want the global namespace
			(v ~= _G.packages) and  -- don't want any packages
			(v ~= _G[".get"]) and
			(v ~= g_APIDesc)
		) then
			if (type(v) == "table") then
				table.insert(API, ParseClass(i, v));
			else
				Add(Globals, i, v);
			end
		end
	end
	
	return API, Globals;
end





function DumpAPIHtml()
	LOG("Dumping all available functions and constants to API subfolder...");

	local API, Globals = CreateAPITables();
	
	-- Sort the classes by name:
	table.sort(API,
		function (c1, c2)
			return (string.lower(c1.Name) < string.lower(c2.Name));
		end
	);
	
	-- Add Globals into the API:
	Globals.Name = "Globals";
	table.insert(API, Globals);
	
	-- Read in the descriptions:
	ReadDescriptions(API);
	
	-- Create a "class index" file, write each class as a link to that file,
	-- then dump class contents into class-specific file
	local f = io.open("API/index.html", "w");
	if (f == nil) then
		-- Create the output folder
		os.execute("mkdir API");
		local err;
		f, err = io.open("API/index.html", "w");
		if (f == nil) then
			LOGINFO("Cannot output HTML API: " .. err);
			return;
		end
	end
	
	f:write([[<html><head><title>MCServer API - class index</title>
	<link rel="stylesheet" type="text/css" href="main.css" />
	</head><body>
	<ul>
	]]);
	for i, cls in ipairs(API) do
		f:write("<li><a href=\"" .. cls.Name .. ".html\">" .. cls.Name .. "</a></li>\n");
		WriteHtmlClass(cls, API);
	end
	f:write("</ul></body></html>");
	f:close();
	
	-- Copy the CSS file to the output folder (overwrite any existing):
	cssf = io.open("API/main.css", "w");
	if (cssf ~= nil) then
		cssfi = io.open(g_Plugin:GetLocalDirectory() .. "/main.css", "r");
		if (cssfi ~= nil) then
			local CSS = cssfi:read("*all");
			cssf:write(CSS);
			cssfi:close();
		end
		cssf:close();
	end
	
	LOG("API subfolder written");
end





function ReadDescriptions(a_API)
	-- Returns true if the function (specified by its fully qualified name) is to be ignored
	local function IsFunctionIgnored(a_FnName)
		for i, name in ipairs(g_APIDesc.IgnoreFunctions) do
			if (a_FnName:match(name)) then
				return true;
			end
		end
		return false;
	end
	
	local UnexportedDocumented = {};  -- List of API objects that are documented but not exported, simply a list of names
	
	for i, cls in ipairs(a_API) do
		-- Rename special functions:
		for j, fn in ipairs(cls.Functions) do
			if (fn.Name == ".call") then
				fn.DocID = "constructor";
				fn.Name = "() <i>(constructor)</i>";
			elseif (fn.Name == ".add") then
				fn.DocID = "operator_plus";
				fn.Name = "<i>operator +</i>";
			elseif (fn.Name == ".div") then
				fn.DocID = "operator_div";
				fn.Name = "<i>operator /</i>";
			elseif (fn.Name == ".mul") then
				fn.DocID = "operator_mul";
				fn.Name = "<i>operator *</i>";
			elseif (fn.Name == ".sub") then
				fn.DocID = "operator_sub";
				fn.Name = "<i>operator -</i>";
			end
		end
		
		local APIDesc = g_APIDesc.Classes[cls.Name];
		if (APIDesc ~= nil) then
			cls.Desc = APIDesc.Desc;
			
			-- Process inheritance:
			if (APIDesc.Inherits ~= nil) then
				for j, icls in ipairs(a_API) do
					if (icls.Name == APIDesc.Inherits) then
						table.insert(icls.Descendants, cls);
						cls.Inherits = icls;
					end
				end
			end
			
			if (APIDesc.Functions ~= nil) then
				-- Assign function descriptions:
				for j, func in ipairs(cls.Functions) do
					local FnName = func.DocID or func.Name;
					local FnDesc = APIDesc.Functions[FnName];
					if (FnDesc ~= nil) then
						func.Params = FnDesc.Params;
						func.Return = FnDesc.Return;
						func.Notes  = FnDesc.Notes;
						FnDesc.IsExported = true;
					end
				end  -- for j, func
				
				-- Add all non-exported function descriptions to UnexportedDocumented:
				for j, func in pairs(APIDesc.Functions) do
					-- TODO
				end
			end  -- if (APIDesc.Functions ~= nil)
			
			if (APIDesc.Constants ~= nil) then
				-- Assign constant descriptions:
				for j, cons in ipairs(cls.Constants) do
					local CnDesc = APIDesc.Constants[cons.Name];
					if (CnDesc ~= nil) then
						cons.Notes = CnDesc.Notes;
						CnDesc.IsExported = true;
					end
				end  -- for j, cons

				-- Add all non-exported constant descriptions to UnexportedDocumented:
				for j, cons in pairs(APIDesc.Constants) do
					-- TODO
				end
			end  -- if (APIDesc.Constants ~= nil)
		end  -- if (APIDesc ~= nil)
		
		-- Remove ignored functions:
		local NewFunctions = {};
		for j, fn in ipairs(cls.Functions) do
			if (not(IsFunctionIgnored(cls.Name .. "." .. fn.Name))) then
				table.insert(NewFunctions, fn);
			end
		end  -- for j, fn
		cls.Functions = NewFunctions;

		-- Sort the functions (they may have been renamed):
		table.sort(cls.Functions,
			function(f1, f2)
				return (f1.Name < f2.Name);
			end
		);
		
		-- Sort the constants:
		table.sort(cls.Constants,
			function(c1, c2)
				return (c1.Name < c2.Name);
			end
		);
	end  -- for i, cls
	
	-- Sort the descendants lists:
	for i, cls in ipairs(a_API) do
		table.sort(cls.Descendants,
			function(c1, c2)
				return (c1.Name < c2.Name);
			end
		);
	end  -- for i, cls
end





function WriteHtmlClass(a_ClassAPI, a_AllAPI)
	local cf, err = io.open("API/" .. a_ClassAPI.Name .. ".html", "w");
	if (cf == nil) then
		return;
	end
	
	-- Make a link out of anything with the special linkifying syntax {{link|title}}
	local function LinkifyString(a_String)
		local txt = a_String:gsub("{{([^|]*)|([^}]*)}}", "<a href=\"%1.html\">%2</a>")  -- {{link|title}}
		txt = txt:gsub("{{([^|]*)}}", "<a href=\"%1.html\">%1</a>")  -- {{LinkAndTitle}}
		return txt;
	end
	
	-- Writes a table containing all functions in the specified list, with an optional "inherited from" header when a_InheritedName is valid
	local function WriteFunctions(a_Functions, a_InheritedName)
		if (#a_Functions == 0) then
			return;
		end

		if (a_InheritedName ~= nil) then
			cf:write("<h2>Functions inherited from " .. a_InheritedName .. "</h2>");
		end
		cf:write("<table><tr><th>Name</th><th>Parameters</th><th>Return value</th><th>Notes</th></tr>\n");
		for i, func in ipairs(a_Functions) do
			cf:write("<tr><td>" .. func.Name .. "</td>");
			cf:write("<td>" .. LinkifyString(func.Params or "").. "</td>");
			cf:write("<td>" .. LinkifyString(func.Return or "").. "</td>");
			cf:write("<td>" .. LinkifyString(func.Notes or "") .. "</td></tr>\n");
		end
		cf:write("</table>\n");
	end
	
	local function WriteDescendants(a_Descendants)
		if (#a_Descendants == 0) then
			return;
		end
		cf:write("<ul>");
		for i, desc in ipairs(a_Descendants) do
			cf:write("<li><a href=\"".. desc.Name .. ".html\">" .. desc.Name .. "</a>");
			WriteDescendants(desc.Descendants);
			cf:write("</li>\n");
		end
		cf:write("</ul>\n");
	end

	-- Build an array of inherited classes chain:
	local InheritanceChain = {};
	local CurrInheritance = a_ClassAPI.Inherits;
	while (CurrInheritance ~= nil) do
		table.insert(InheritanceChain, CurrInheritance);
		CurrInheritance = CurrInheritance.Inherits;
	end
	
	cf:write([[<html><head><title>MCServer API - ]] .. a_ClassAPI.Name .. [[</title>
	<link rel="stylesheet" type="text/css" href="main.css" />
	</head><body>
	<h1>Contents</h1>
	<ul>
	]]);
	
	local HasInheritance = ((#a_ClassAPI.Descendants > 0) or (a_ClassAPI.Inherits ~= nil));
	
	-- Write the table of contents:
	if (HasInheritance) then
		cf:write("<li><a href=\"#inherits\">Inheritance</a></li>\n");
	end
	cf:write("<li><a href=\"#constants\">Constants</a></li>\n");
	cf:write("<li><a href=\"#functions\">Functions</a></li>\n");
	cf:write("</ul>");
	
	-- Write the class description:
	cf:write("<a name=\"desc\"><h1>" .. a_ClassAPI.Name .. " class</h1></a>\n");
	if (a_ClassAPI.Desc ~= nil) then
		cf:write("<p>");
		cf:write(LinkifyString(a_ClassAPI.Desc));
		cf:write("</p>\n");
	end;
	
	-- Write the inheritance, if available:
	if (HasInheritance) then
		cf:write("<a name=\"inherits\"><h1>Inheritance</h1></a>\n");
		if (#InheritanceChain > 0) then
			cf:write("<p>This class inherits from the following parent classes:<ul>\n");
			for i, cls in ipairs(InheritanceChain) do
				cf:write("<li><a href=\"" .. cls.Name .. ".html\">" .. cls.Name .. "</a></li>\n");
			end
			cf:write("</ul></p>\n");
		end
		if (#a_ClassAPI.Descendants > 0) then
			cf:write("<p>This class has the following descendants:\n");
			WriteDescendants(a_ClassAPI.Descendants);
			cf:write("</p>\n");
		end
	end
	
	-- Write the constants:
	cf:write("<a name=\"constants\"><h1>Constants</h1></a>\n");
	cf:write("<table><tr><th>Name</th><th>Value</th><th>Notes</th></tr>\n");
	for i, cons in ipairs(a_ClassAPI.Constants) do
		cf:write("<tr><td>" .. cons.Name .. "</td>");
		cf:write("<td>" .. cons.Value .. "</td>");
		cf:write("<td>" .. LinkifyString(cons.Notes or "") .. "</td></tr>\n");
	end
	cf:write("</table>\n");
	
	-- Write the functions, including the inherited ones:
	cf:write("<a name=\"functions\"><h1>Functions</h1></a>\n");
	WriteFunctions(a_ClassAPI.Functions, nil);
	for i, cls in ipairs(InheritanceChain) do
		WriteFunctions(cls.Functions, cls.Name);
	end
	
	cf:write("</body></html>");
	cf:close();
end





