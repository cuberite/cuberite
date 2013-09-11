
-- main.lua

-- Implements the plugin entrypoint (in this case the entire plugin)





-- Global variables:
g_Plugin = nil;






function Initialize(Plugin)
	g_Plugin = Plugin;
	
	Plugin:SetName("APIDump")
	Plugin:SetVersion(1)
	
	PluginManager = cRoot:Get():GetPluginManager()
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())

	-- dump all available API functions and objects:
	-- DumpAPITxt();
	
	-- DEBUG: Convert the wiki dump into APIDesc
	ConvertWikiToDesc();
	
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

	local Globals = {Functions = {}, Constants = {}};
	local API = {};
	
	local function Add(a_APIContainer, a_ClassName, a_ClassObj)
		if (type(a_ClassObj) == "function") then
			table.insert(a_APIContainer.Functions, {Name = a_ClassName});
		elseif (type(a_ClassObj) == "number") then
			table.insert(a_APIContainer.Constants, {Name = a_ClassName, Value = a_ClassObj});
		end
	end
	
	local function SortClass(a_ClassAPI)
		-- Sort the function list and constant lists:
		table.sort(a_ClassAPI.Functions,
			function(f1, f2)
				return (f1.Name < f2.Name);
			end
		);
		table.sort(a_ClassAPI.Constants,
			function(c1, c2)
				return (c1.Name < c2.Name);
			end
		);
	end;
	
	local function ParseClass(a_ClassName, a_ClassObj)
		local res = {Name = a_ClassName, Functions = {}, Constants = {}};
		for i, v in pairs(a_ClassObj) do
			Add(res, i, v);
		end
		
		SortClass(res);
		return res;
	end
	
	for i, v in pairs(_G) do
		if (type(v) == "table") then
			-- It is a table - probably a class
			local StartLetter = GetChar(i, 0);
			if (StartLetter == "c") then
				-- Starts with a "c", handle it as a MCS API class
				table.insert(API, ParseClass(i, v));
			end
		else
			Add(Globals, i, v);
		end
	end
	SortClass(Globals);
	table.sort(API,
		function(c1, c2)
			return (c1.Name < c2.Name);
		end
	);
	
	return API, Globals;
end





function DumpAPIHtml()
	LOG("Dumping all available functions and constants to API subfolder...");

	local API, Globals = CreateAPITables();
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
		WriteHtmlClass(cls);
	end
	f:write("</ul></body></html>");
	f:close();
	
	LOG("API subfolder written");
end





function ReadDescriptions(a_API)
	local UnexportedDocumented = {};  -- List of API objects that are documented but not exported, simply a list of names
	for i, cls in ipairs(a_API) do
		local APIDesc = g_APIDesc.Classes[cls.Name];
		if (APIDesc ~= nil) then
			cls.Desc = APIDesc.Desc;
			
			if (APIDesc.Functions ~= nil) then
				-- Assign function descriptions:
				for j, func in ipairs(cls.Functions) do
					-- func is {"FuncName"}, add Parameters, Return and Notes from g_APIDesc
					local FnDesc = APIDesc.Functions[func.Name];
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
			
		end
	end  -- for i, class
end





function WriteHtmlClass(a_ClassAPI)
	local cf, err = io.open("API/" .. a_ClassAPI.Name .. ".html", "w");
	if (cf == nil) then
		return;
	end
	
	local function LinkifyString(a_String)
		-- TODO: Make a link out of anything with the special linkifying syntax {{link|title}}
		-- a_String:gsub("{{([^|]*)|[^}]*}}", "<a href=\"%1\">%2</a>");
		return a_String;
	end
	
	cf:write([[<html><head><title>MCServer API - ]] .. a_ClassAPI.Name .. [[</title>
	<link rel="stylesheet" type="text/css" href="main.css" />
	</head><body>
	<h1>Contents</h1>
	<ul>
	]]);
	
	-- Write the table of contents:
	if (#a_ClassAPI.Constants > 0) then
		cf:write("<li><a href=\"#constants\">Constants</a></li>\n");
	end
	if (#a_ClassAPI.Functions > 0) then
		cf:write("<li><a href=\"#functions\">Functions</a></li>\n");
	end
	cf:write("</ul>");
	
	-- Write the class description:
	cf:write("<a name=\"desc\"><h1>" .. a_ClassAPI.Name .. "</h1></a>\n");
	if (a_ClassAPI.Desc ~= nil) then
		cf:write("<p>");
		cf:write(a_ClassAPI.Desc);
		cf:write("</p>\n");
	end;
	
	-- Write the constants:
	if (#a_ClassAPI.Constants > 0) then
		cf:write("<a name=\"constants\"><h1>Constants</h1></a>\n");
		cf:write("<table><tr><th>Name</th><th>Value</th><th>Notes</th></tr>\n");
		for i, cons in ipairs(a_ClassAPI.Constants) do
			cf:write("<tr><td>" .. cons.Name .. "</td>");
			cf:write("<td>" .. cons.Value .. "</td>");
			cf:write("<td>" .. LinkifyString(cons.Notes or "") .. "</td></tr>\n");
		end
		cf:write("</table>\n");
	end
	
	-- Write the functions:
	if (#a_ClassAPI.Functions > 0) then
		cf:write("<a name=\"functions\"><h1>Functions</h1></a>\n");
		cf:write("<table><tr><th>Name</th><th>Parameters</th><th>Return value</th><th>Notes</th></tr>\n");
		for i, func in ipairs(a_ClassAPI.Functions) do
			cf:write("<tr><td>" .. func.Name .. "</td>");
			cf:write("<td>" .. LinkifyString(func.Params or "").. "</td>");
			cf:write("<td>" .. LinkifyString(func.Return or "").. "</td>");
			cf:write("<td>" .. LinkifyString(func.Notes or "") .. "</td></tr>\n");
		end
		cf:write("</table>\n");
	end
	
	cf:write("</body></html>");
	cf:close();
end





-- This function converts the wiki dump, as provided by FakeTruth, into the APIDesc format.
-- Dump available in forum: http://forum.mc-server.org/showthread.php?tid=1214&pid=9892#pid9892
-- The dump is expected unpacked as "wikipages/api/*.txt", in the executable folder
-- Only Windows-style paths are supported for now, since this is a one-time action
function ConvertWikiToDesc()
	local fout = io.open("APIDesc.wiki.lua", "w");
	for filename in io.popen([[dir wikipages\\api\\*.txt /b]]):lines() do
		-- Read file
		local fin = io.open("wikipages\\api\\" .. filename, "r");
		if (fin ~= nil) then
			-- Read and parse the info from the file
			local state = 0;
			local Desc = "";
			local Constants = {};
			local Functions = {};
			for line in fin:lines() do
				if (line:find("======") ~= nil) then
					state = 1;  -- The following is the class description
				elseif (line:find("===== Constants") ~= nil) then
					state = 2;  -- The following is the constants description
				elseif (line:find("===== Functions") ~= nil) then
					state = 3;  -- The following is the functions description
				elseif (line:find("=====") ~= nil) then
					state = 4;  -- The following is an unknown text, skip it entirely
				elseif (state == 1) then
					-- Class description:
					if (line == "") then
						line = "</p>\n<p>";  -- Replace empty lines with paragraph delimiters
					end
					Desc = Desc .. line .. "\n";
				elseif (state == 2) then
					-- Constants:
					local Split = StringSplitAndTrim(line, "|");
					if (#Split >= 3) then
						-- Split[1] is always "", because the line starts with a "|"
						table.insert(Constants, {Name = Split[2], Notes = Split[3]});
					end
				elseif (state == 3) then
					-- Functions:
					local Split = StringSplitAndTrim(line, "|");
					if (#Split >= 5) then
						-- Split[1] is always "", because the line starts with a "|"
						table.insert(Functions, {Name = Split[2], Params = Split[3], Return = Split[4], Notes = Split[5]});
					end
				end
			end  -- for line
			fin:close();
			
			-- Write the info into the output file:
			fout:write(filename:match("[^\.]*") .. " =\n{\tFunctions =\n\t{\n");
			for i, func in ipairs(Functions) do
				fout:write("\t\t{ " .. func.Name .. " = { Params = \"" .. func.Params .. "\", Return =\"" ..
					func.Return .. "\", Desc = \"" .. func.Notes .. "\" },\n"
				);
			end
			fout:write("\t},\n\tConstants =\n\t{\n");
			for i, cons in ipairs(Constants) do
				fout:write("\t\t{ " .. cons.Name .. " = { Notes = \"" .. cons.Notes .. "\" },\n");
			end
			fout:write("\t},\n},\n\n\n");
		end  -- if fin ~= nil
	end  -- for file
	fout:close();
end




