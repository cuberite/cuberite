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

	Content = Content .. "</ul><br>";

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
	<link rel="stylesheet" href="/css/fonts.css">
	<link rel="icon" type="image/png" href="/icons/favicon-32x32.png" sizes="32x32">
	<link rel="icon" type="image/png" href="/icons/favicon-16x16.png" sizes="16x16">
	<link rel="manifest" href="/icons/manifest.json">
	<link rel="mask-icon" href="/icons/safari-pinned-tab.svg">
	<meta name="theme-color" content="#245A48">
	<link rel="stylesheet" href="/css/normalize.css">
	<link rel="stylesheet" href="/css/style.css">
</head>
<body>
	<div id="navigation">
		<div id="logo">
			<a href="/webadmin/">
				<svg aria-hidden="true" height="1.125em" viewBox="0 0 24 24" width="1.125em">
					<path d="M6 3.04l-6 3v11.92l6 3 6 3 6-3 6-3V6.03l-6-3-6.02-3-6 3zm5.05 4.58l.96.47.96-.5.96-.47V5.96c0-.64.02-1.18.05-1.2.08-.03 4.24 2.12 4.24 2.2 0 .03-1.4.76-3.1 1.63L12 10.14 9.05 8.67C7.4 7.85 5.98 7.13 5.87 7.05l-.2-.12 2.18-1.1 2.2-1.1.02 1.2v1.2zm-4.08 3.72l3.1 1.58v6.34l-2.2-1.1c-1.2-.6-2.2-1.1-2.2-1.13 0-.02.57-.3 1.25-.65l1.25-.6v-1.75l-1-.5-1-.5-1.16.6-1.2.58c-.02 0-.04-1-.04-2.23s.02-2.24.05-2.24c.03 0 1.45.7 3.17 1.58zm13.23.68l-.06 2.2-1.15-.58-1.18-.58-1 .5-1 .5v.74c0 .4.04.8.06.9.03.08.34.28.94.57.5.25 1 .53 1.14.65l.24.2-2.15 1.08-2.13 1.08v-6.4l3.14-1.54c1.73-.86 3.16-1.55 3.16-1.54.02 0 0 1-.02 2.22z" />
				</svg>
				<span>WebAdmin</span>
			</a>
		</div>
		<div id="navigation-links">
			<a><span><strong>]] .. NumPlayers .. [[</strong> Players</span></a>
			<a><span><strong>]] .. string.format("%.2f", MemoryUsageKiB / 1024) .. [[MB</strong> Memory</span></a>
			<a><span><strong>]] .. NumChunks .. [[</strong> Chunks</span></a>
			<a href=".././">
				<svg width="1.313em" height="1.313em" viewBox="0 0 16 16" version="1.1" aria-hidden="true">
					<path fill-rule="evenodd" d="M12 9V7H8V5h4V3l4 3-4 3zm-2 3H6V3L2 1h8v3h1V1c0-.55-.45-1-1-1H1C.45 0 0 .45 0 1v11.38c0 .39.22.73.55.91L6 16.01V13h4c.55 0 1-.45 1-1V8h-1v4z"></path>
				</svg>
				<span>Logout</span>
			</a>
		</div>
	</div>

	<div id="main">
		<div id="main-body">
			<div id="container">
				<div id="left-column">
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
		Output("<h2>" .. pluginWebTitle .. "</h2>");

		Output("<ul>\n");
		-- Output each tab:
		for _, tab in pairs(pluginTabs) do
			Output("<li><a href='" .. BaseURL .. pluginName .. "/" .. tab.UrlPath .. "'>" .. tab.Title .. "</a></li>\n")
		end
		Output("</ul>\n");
	end

	Output([[	</div>
				<div id="right-column">
					<h1><span>]] .. SubTitle .. [[</span></h1>
					]] .. PageContent .. [[
				</div>
			</div>
		</div>
	</div>

	<input type="checkbox" id="footer-links-toggle"/>
	<div id="footer">
		<div id="mobile-links">
			<a><strong>]] .. NumPlayers .. [[</strong>Players</span></a>
			<a><strong>]] .. string.format("%.2f", MemoryUsageKiB / 1024) .. [[MB</strong>Memory</span></a>
			<a><strong>]] .. NumChunks .. [[</strong>Chunks</span></a>
			<a href=".././">
				<svg aria-hidden="true" viewBox="0 0 24 24">
					<path d="M20 18.05V8c0-2.4-2.4-4.8-4.8-4.8h-1.6V0L8.8 4.8l4.8 4.8V6.4h1.6c.8 0 1.6.8 1.6 1.6v10.05c-.95.55-1.6 1.57-1.6 2.75 0 1.78 1.42 3.2 3.2 3.2 1.77 0 3.2-1.42 3.2-3.2 0-1.18-.65-2.2-1.6-2.75zm-1.6 4.35c-.88 0-1.6-.73-1.6-1.6 0-.88.72-1.6 1.6-1.6.87 0 1.6.72 1.6 1.6 0 .87-.73 1.6-1.6 1.6zM5.6 1.6c-1.77 0-3.2 1.42-3.2 3.2 0 1.18.65 2.2 1.6 2.75v10.47c-.95.56-1.6 1.58-1.6 2.76 0 1.77 1.43 3.2 3.2 3.2 1.78 0 3.2-1.43 3.2-3.2 0-1.18-.65-2.2-1.6-2.76V7.55C8.15 7 8.8 5.98 8.8 4.8c0-1.78-1.42-3.2-3.2-3.2zm0 20.8c-.87 0-1.6-.73-1.6-1.6 0-.88.73-1.6 1.6-1.6.88 0 1.6.72 1.6 1.6 0 .87-.72 1.6-1.6 1.6zm0-16C4.73 6.4 4 5.68 4 4.8c0-.88.73-1.6 1.6-1.6.88 0 1.6.72 1.6 1.6 0 .88-.72 1.6-1.6 1.6z" />
				</svg>
				<span>Logout</span>
			</a>

			<label for="footer-links-toggle">
				<svg aria-hidden="true" viewBox="0 0 24 24">
					<path d="M6.55 10.36v3.28q0 .68-.48 1.15-.48.47-1.16.47H1.65q-.7 0-1.16-.48Q0 14.3 0 13.63v-3.28q0-.68.48-1.15.48-.47 1.16-.47H4.9q.7 0 1.17.48t.48 1.16zm8.72 0v3.28q0 .68-.48 1.15-.48.47-1.16.47h-3.28q-.68 0-1.15-.48-.47-.48-.47-1.16v-3.28q0-.68.48-1.15.48-.47 1.16-.47h3.28q.68 0 1.15.48.47.48.47 1.16zm8.73 0v3.28q0 .68-.48 1.15-.48.47-1.16.47H19.1q-.7 0-1.17-.48t-.48-1.16v-3.28q0-.68.48-1.15.48-.47 1.16-.47h3.26q.7 0 1.16.48.48.48.48 1.16z" />
				</svg>
				Menu
			</label>
		</div>
		<div id="footer-links">
			<a href="https://cuberite.org/" target="_blank">Website</a>
			<a href="https://cuberite.org/news/" target="_blank">News</a>
			<a href="https://forum.cuberite.org/" target="_blank">Forums</a>
			<a href="https://api.cuberite.org/" target="_blank">API</a>
			<a href="https://cuberite.org/contribute/" target="_blank">Contribute</a>
			<a href="https://salt.bountysource.com/teams/cuberite" target="_blank">Donate</a>
		</div>
	</div>
</body>
</html>
]])

	return table.concat(SiteContent)
end
