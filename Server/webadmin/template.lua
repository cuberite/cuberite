-- Use a table for fast concatenation of strings
local SiteContent = {}
function Output(String)
	table.insert(SiteContent, String)
end





function GetTableSize(Table)
	local Size = 0
	for key,value in pairs(Table) do
		Size = Size + 1
	end
	return Size
end





local function GetDefaultPage()
	local PM = cRoot:Get():GetPluginManager()

	local SubTitle = "Current Game"
	local Content = ""

	Content = Content .. "<h4>Plugins:</h4><ul>"
	PM:ForEachPlugin(
		function (a_CBPlugin)
			if (a_CBPlugin:IsLoaded()) then
				Content = Content ..  "<li>" .. a_CBPlugin:GetName() .. " (version " .. a_CBPlugin:GetVersion() .. ")</li>"
			end
		end
	)

	Content = Content .. "</ul>"
	Content = Content .. "<h4>Players:</h4><ul>"

	cRoot:Get():ForEachPlayer(
		function(a_CBPlayer)
			Content = Content .. "<li>" .. a_CBPlayer:GetName() .. "</li>"
		end
	)

	Content = Content .. "</ul>";

	return Content, SubTitle
end





function ShowPage(WebAdmin, TemplateRequest)
	SiteContent = {}
	local BaseURL = cWebAdmin:GetBaseURL(TemplateRequest.Request.Path)
	local Title = "Cuberite WebAdmin"
	local NumPlayers = cRoot:Get():GetServer():GetNumPlayers()
	local MemoryUsageKiB = cRoot:GetPhysicalRAMUsage()
	local NumChunks = cRoot:Get():GetTotalChunkCount()
	local PluginPage = cWebAdmin:GetPage(TemplateRequest.Request)
	local PageContent = PluginPage.Content
	local SubTitle = PluginPage.PluginFolder
	if (PluginPage.UrlPath ~= "") then
		SubTitle = PluginPage.PluginFolder .. " - " .. PluginPage.TabTitle
	end
	if (PageContent == "") then
		PageContent, SubTitle = GetDefaultPage()
	end

	--[[
	-- 2016-01-15 Mattes: This wasn't used anywhere in the code, no idea what it was supposed to do
	local reqParamsClass = ""
	for key, value in pairs(TemplateRequest.Request.Params) do
		reqParamsClass = reqParamsClass .. " param-" .. string.lower(string.gsub(key, "[^a-zA-Z0-9]+", "-") .. "-" .. string.gsub(value, "[^a-zA-Z0-9]+", "-"))
	end
	if (string.gsub(reqParamsClass, "%s", "") == "") then
		reqParamsClass = " no-param"
	end
	--]]

	Output([[
<!-- Copyright Justin S and Cuberite Team, licensed under CC-BY-SA 3.0 -->
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>]] .. Title .. [[</title>
	<link rel="stylesheet" type="text/css" href="/style.css">
</head>
<body>
<div class="header color-background">
	<div class="wrapper">
		<a href="]] .. BaseURL .. [[">
			<svg viewBox="0 0 1716.3687 336.90039" width="18em">
				<path d="M523.63 222.72q30.36 0 49.64-24.02l26.4 27.2q-31.42 35.37-74.2 35.37-42.5 0-70.2-26.93-27.47-26.93-27.47-67.85 0-41.2 28-68.65 28.23-27.46 68.9-27.46 45.4 0 76.02 34.58l-25.6 29.04q-19.54-24.3-48.58-24.3-23.23 0-39.87 15.32-16.36 15.05-16.36 40.92 0 25.6 15.58 41.2 15.58 15.56 37.75 15.56z"></path><path d="M674.82 211.63q10.3 12.68 27.72 12.68 17.42 0 27.45-12.67 10.3-12.93 10.3-35.1V74.62h41.17v103.2q0 40.14-22.17 61.8Q737.12 261 702.54 261q-34.6 0-57.03-21.65-22.16-21.64-22.16-61.5V74.6h41.18v101.9q0 22.18 10.3 35.1z"></path><path d="M899 259.15h-80.78V74.62h71.8q18.75 0 32.22 4.5 13.73 4.47 20.6 12.13 12.4 14.26 12.4 32.2 0 21.66-14 32.22-4.75 3.7-6.6 4.75-1.84.8-6.6 2.9 17.17 3.7 27.2 15.58 10.3 11.6 10.3 29.04 0 19.27-13.2 34.05Q937 259.14 899 259.14zm-39.6-109.82h19.54q17.16 0 25.35-3.7 8.43-3.7 8.43-15.84 0-12.4-7.65-16.37-7.66-3.96-25.87-3.96h-19.8v39.86zm0 74.98h28.25q17.7 0 26.4-4.22 8.98-4.5 8.98-17.16 0-12.93-9.5-16.9-9.24-4.22-29.84-4.22h-24.3v42.5z"></path><path d="M1127.02 74.62v36.7h-91.87v38h82.63v35.12h-82.63v38.28h94.78v36.43H993.97V74.62h133.05z"></path><path d="M1310.12 135.87q0 44.35-35.12 57.28l46.73 66h-50.7l-40.9-58.87h-28.52v58.87h-41.17V74.62h69.96q43.02 0 61.24 14.52 18.48 14.52 18.48 46.73zm-78.4 28.77q21.1 0 28.76-6.86 7.66-6.87 7.66-21.65 0-15.05-7.92-20.6-7.92-5.54-27.72-5.54h-30.9v54.64h30.1z"></path><path d="M1345.97 74.62h41.18v184.53h-41.18V74.62z"></path><path d="M1504.47 110.26v148.9h-41.2v-148.9h-52.26V74.62h145.74v35.64h-52.27z"></path><path d="M1713.46 74.62v36.7h-91.87v38h82.62v35.12h-82.63v38.28h94.77v36.43H1580.4V74.62h133.06z"></path><path d="M84.3 42.2L0 84.4v168l84.5 42.3 84.5 42.2 84.5-42.2 84.5-42.3v-168l-84.5-42.3C207 18.9 168.9-.1 168.7 0c-.1 0-38.1 19-84.4 42.2zm71.3 64.5l13.5 6.7 13.4-6.8 13.5-6.8V83.4c0-9 .3-16.6.8-16.8 1.1-.5 59.7 29.7 59.6 30.8-.1.5-19.8 10.8-43.7 23l-43.6 22-41.8-20.9c-23-11.5-43.1-21.7-44.7-22.7L79.7 97l30.9-15.4 30.9-15.5.3 16.9.2 16.9 13.6 6.8zm-57.4 52.4l43.8 22.3v89.5l-31-15.5c-17.1-8.5-31-15.7-31-16 0-.3 7.9-4.4 17.5-9.1l17.5-8.5v-24.5l-14-6.9-14-7-16.5 8.3c-9.1 4.5-16.7 8.2-17 8.2-.3 0-.5-14.2-.5-31.5s.3-31.5.7-31.5c.3 0 20.3 10 44.5 22.2zm186.3 9.7l-.8 31-16.3-8.2-16.4-8.2-14 7-14 6.9v10.6c0 5.8.3 11.4.6 12.5.4 1.3 4.9 4.1 13.3 8.2 6.9 3.5 14.1 7.5 16 9.1l3.4 2.8-30.2 15.3L196 271v-90l44.3-21.9c24.3-12.1 44.4-21.8 44.5-21.7.2.2.1 14.3-.3 31.4z"></path>
			</svg>
		</a>
	</div>
</div>
<div class="panel">
	<div class="wrapper">
		<div class="welcome"><strong>Welcome back, ]] .. TemplateRequest.Request.Username .. [[</strong>&nbsp;&nbsp;&nbsp;<a href="/" class="link-logout">Log Out</a></div>
		<ul class="menu top-links">
			<li>Players online: <strong>]] .. NumPlayers .. [[</strong></li>
			<li>Memory: <strong>]] .. string.format("%.2f", MemoryUsageKiB / 1024) .. [[MB</strong></li>
			<li>Chunks: <strong>]] .. NumChunks .. [[</strong></li>
		</ul>
	</div>
</div>
<div class="columns">
	<div class="wrapper">
		<div class="tborder left">
			<h2 class="thead color-background">Menu</h2>
			<div class="trow"><a href="]] .. BaseURL .. [[" class="sidebar-item link-home">Home</a></div>
			<div class="tcat">Server Management</div>
			<div class="trow">
	]])

	-- Get all tabs:
	local perPluginTabs = {}
	for _, tab in ipairs(cWebAdmin:GetAllWebTabs()) do
		local pluginTabs = perPluginTabs[tab.PluginName] or {};
		perPluginTabs[tab.PluginName] = pluginTabs
		table.insert(pluginTabs, tab)
	end
	
	-- Sort by plugin:
	local pluginNames = {}
	for pluginName, pluginTabs in pairs(perPluginTabs) do
		table.insert(pluginNames, pluginName)
	end
	table.sort(pluginNames)
	
	-- Output by plugin, then alphabetically:
	for _, pluginName in ipairs(pluginNames) do
		local pluginTabs = perPluginTabs[pluginName]
		table.sort(pluginTabs,
			function(a_Tab1, a_Tab2)
				return ((a_Tab1.Title or "") < (a_Tab2.Title or ""))
			end
		)
		
		-- Translate the plugin name into the folder name (-> title)
		local pluginWebTitle = cPluginManager:Get():GetPluginFolderName(pluginName) or pluginName
		Output("<div class=\"sidebar-item link-page\"><strong>" .. pluginWebTitle .. "</strong></div>\n");

		-- Output each tab:
		for _, tab in pairs(pluginTabs) do
			Output("<div><a href=\"" .. BaseURL .. pluginName .. "/" .. tab.UrlPath .. "\" class=\"sidebar-item link-subpage\">" .. tab.Title .. "</a></div>\n")
		end
		Output("\n");
	end


	Output([[
			</div>
		</div>
		<div class="tborder">
			<h1 class="thead color-background">]] .. SubTitle .. [[</h1>
			<div class="tbody">]] .. PageContent .. [[</div>
		</div>
	</div>
</div>
<div class="footer">
	<div class="upper">
		<div class="wrapper">
			<ul class="menu bottom-links">
				<li><a href="https://cuberite.org/" target="_blank">Cuberite</a></li>
				<li><a href="https://forum.cuberite.org/" target="_blank">Forums</a></li>
				<li><a href="https://builds.cuberite.org/" target="_blank">Buildserver</a></li>
				<li><a href="https://api.cuberite.org/" target="_blank">API Documentation</a></li>
				<li><a href="https://book.cuberite.org/" target="_blank">User's Manual</a></li>
			</ul>
		</div>
	</div>
	<div class="lower color-background">
		<div class="wrapper">
			<span id="copyright">Copyright © <a href="https://cuberite.org/" target="_blank">Cuberite Team</a>.</span>
		</div>
	</div>
</div>
</body>
</html>
]])

	return table.concat(SiteContent)
end
