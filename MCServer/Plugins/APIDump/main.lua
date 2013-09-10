
-- main.lua

-- Implements the plugin entrypoint (in this case the entire plugin)






function Initialize(Plugin)
	Plugin:SetName("APIDump")
	Plugin:SetVersion(1)
	
	PluginManager = cRoot:Get():GetPluginManager()
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())

	-- dump all available API functions and objects:
	-- DumpAPITxt();
	
	-- Dump all available API objects in wiki-style tables:
	-- DumpAPIWiki();
	
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




function DumpAPIWiki()
	LOG("Dumping all available functions and constants to API_wiki.txt...");

	local API, Globals = CreateAPITables();

	-- Now dump the whole thing into a file, formatted as a wiki table:
	local function WriteClass(a_File, a_ClassAPI)
		if (#a_ClassAPI.Functions > 0) then
			a_File:write("Functions:\n");
			a_File:write("^ Function name ^ Parameters ^ Return value ^ Note ^\n");
			for i, n in ipairs(a_ClassAPI.Functions) do
				a_File:write("| " .. n[1] .. " | | | |\n");
			end
			a_File:write("\n\n");
		end
		
		if (#a_ClassAPI.Constants > 0) then
			a_File:write("Constants:\n");
			a_File:write("^ Constant ^ Value ^ Note ^\n");
			for i, n in ipairs(a_ClassAPI.Constants) do
				a_File:write("| " .. n[1] .. " | " .. n[2] .. " | |\n");
			end
			a_File:write("\n\n");
		end
	end
	
	local f = io.open("API_wiki.txt", "w");
	for i, n in ipairs(API) do
		f:write("Class " .. n[1] .. "\n");
		WriteClass(f, n[2]);
		f:write("\n\n\n----------------\n");
	end
	f:write("globals:\n");
	WriteClass(f, Globals);
	f:close();
	
	LOG("API_wiki.txt file written");
end




function CreateAPITables()
	--[[
	We want an API table of the following shape:
	local API = {
		{"cCuboid", {
			Functions = {
				{"Sort"},    -- The extra table will be used to later add params, return values and notes
				{"IsInside"}
			},
			Constants = {
			}
		}},
		{"cBlockArea", {
			Functions = {
				"Clear",
				"CopyFrom",
				...
			}
			Constants = {
				{"baTypes", 0},
				{"baMetas", 1},
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
			table.insert(a_APIContainer.Functions, {a_ClassName});
		elseif (type(a_ClassObj) == "number") then
			table.insert(a_APIContainer.Constants, {a_ClassName, a_ClassObj});
		end
	end
	
	local function SortClass(a_ClassAPI)
		-- Sort the function list and constant lists:
		table.sort(a_ClassAPI.Functions,
			function(f1, f2)
				return (f1[1] < f2[1]);
			end
		);
		table.sort(a_ClassAPI.Constants,
			function(c1, c2)
				return (c1[1] < c2[1]);
			end
		);
	end;
	
	local function ParseClass(a_ClassObj)
		local res = {Functions = {}, Constants = {}};
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
				table.insert(API, {i, ParseClass(v)});
			end
		else
			Add(Globals, i, v);
		end
	end
	SortClass(Globals);
	table.sort(API,
		function(c1, c2)
			return (c1[1] < c2[1]);
		end
	);
	
	return API, Globals;
end





function DumpAPIHtml()
	LOG("Dumping all available functions and constants to API subfolder...");

	local API, Globals = CreateAPITables();
	
	-- Create the folder:
	os.execute("mkdir API");
	
	-- Create a "class index" file, write each class as a link to that file,
	-- then dump class contents into class-specific file
	local f = io.open("API/index.html", "w");
	f:write([[<html><head><title>MCServer API - class index</title>
	<link rel="stylesheet" type="text/css" href="main.css" />
	</head><body>
	<ul>
	]]);
	for i, n in ipairs(API) do
		f:write("<li><a href=\"" .. n[1] .. ".html\">" .. n[1] .. "</a></li>\n");
		WriteHtmlClass(n);
	end
	f:write("</ul></body></html>");
	f:close();
end




function WriteHtmlClass(a_ClassAPI)
	local cf, err = io.open("API/" .. a_ClassAPI[1] .. ".html", "w");
	if (cf == nil) then
		return;
	end
	
	local function LinkifyString(a_String)
		-- TODO: Make a link out of anything with the special linkifying syntax [[link|title]]
		-- a_String:gsub("\[\[" .. "[
		return a_String;
	end
	
	cf:write([[<html><head><title>MCServer API - ]] .. a_ClassAPI[1] .. [[</title>
	<link rel="stylesheet" type="text/css" href="main.css" />
	</head><body>
	<h1>Contents</h1>
	<ul>
	]]);
	
	-- Write the table of contents:
	if (#a_ClassAPI[2].Constants > 0) then
		cf:write("<li><a href=\"#constants\">Constants</a></li>\n");
	end
	if (#a_ClassAPI[2].Functions > 0) then
		cf:write("<li><a href=\"#functions\">Functions</a></li>\n");
	end
	cf:write("</ul>");
	
	-- Write the class description:
	cf:write("<a name=\"desc\"><h1>" .. a_ClassAPI[1] .. "</h1></a>\n");
	if (a_ClassAPI.Description ~= nil) then
		cf:write("<p>");
		cf:write(n.Description);
		cf:write("</p>\n");
	end;
	
	-- Write the constants:
	if (#a_ClassAPI[2].Constants > 0) then
		cf:write("<a name=\"constants\"><h1>Constants</h1></a>\n");
		cf:write("<table><tr><th>Name</th><th>Value</th><th>Notes</th></tr>\n");
		for i, n in ipairs(a_ClassAPI[2].Constants) do
			cf:write("<tr><td>" .. n[1] .. "</td>");
			cf:write("<td>" .. n[2] .. "</td>");
			cf:write("<td>" .. LinkifyString(n.Notes or "") .. "</td></tr>\n");
		end
		cf:write("</table>\n");
	end
	
	-- Write the functions:
	if (#a_ClassAPI[2].Functions > 0) then
		cf:write("<a name=\"functions\"><h1>Functions</h1></a>\n");
		cf:write("<table><tr><th>Name</th><th>Parameters</th><th>Return value</th><th>Notes</th></tr>\n");
		for i, f in ipairs(a_ClassAPI[2].Functions) do
			cf:write("<tr><td>" .. f[1] .. "</td>");
			cf:write("<td>" .. LinkifyString(f.Params or "").. "</td>");
			cf:write("<td>" .. LinkifyString(f.Return or "").. "</td>");
			cf:write("<td>" .. LinkifyString(f.Notes or "") .. "</td></tr>\n");
		end
		cf:write("</table>\n");
	end
	
	cf:write("</body></html>");
	cf:close();
end




