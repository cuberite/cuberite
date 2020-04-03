-- Use a table for fast concatenation of strings
local SiteContent = {}
function Output(String)
	table.insert(SiteContent, String)
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
		<a href="]] .. BaseURL .. [[" class="logo">Cuberite</a>
	</div>
</div>
<div class="panel">
	<div class="wrapper">
		<div class="welcome">
			<strong>Welcome back, ]] .. TemplateRequest.Request.Username .. [[</strong>
			<a href="/" class="link-logout">Log out</a>
		</div>
		<ul class="stats">
			<li>Players online: <strong>]] .. NumPlayers .. [[</strong></li>
			<li>Memory: <strong>]] .. string.format("%.2f", MemoryUsageKiB / 1024) .. [[MB</strong></li>
			<li>Chunks: <strong>]] .. NumChunks .. [[</strong></li>
		</ul>
	</div>
</div>
<div class="columns">
	<div class="columns-wrapper">
		<div class="columns-spacing">
			<div class="box left">
				<h2 class="head color-background">Menu</h2>
				<ul class="sidebar">
					<li>
						<a href="]] .. BaseURL .. [[" class="link-home">Home</a>
					</li>
				</ul>
				<div class="category">Server Management</div>
				<ul class="sidebar">
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
		Output("<li><strong class=\"link-page\">" .. pluginWebTitle .. "</strong></li>\n");

		-- Output each tab:
		for _, tab in pairs(pluginTabs) do
			Output("<li><a href=\"" .. BaseURL .. pluginName .. "/" .. tab.UrlPath .. "\" class=\"sidebar-item link-subpage\">" .. tab.Title .. "</a></li>\n")
		end
		Output("\n");
	end


	Output([[
				</ul>
			</div>
			<div class="box right">
				<h1 class="head color-background">]] .. SubTitle .. [[</h1>
				<div class="main-content">]] .. PageContent .. [[</div>
			</div>
		</div>
	</div>
</div>
<div class="footer">
	<div class="footer-container">
		<div class="wrapper">
			<span class="copyright">Copyright © <a href="https://cuberite.org/" target="_blank">Cuberite Team</a></span>
			<ul class="footer-links">
				<li><a href="https://cuberite.org/" target="_blank">Cuberite</a></li>
				<li><a href="https://forum.cuberite.org/" target="_blank">Forums</a></li>
				<li><a href="https://api.cuberite.org/" target="_blank">API Docs</a></li>
				<li><a href="https://book.cuberite.org/" target="_blank">User's Manual</a></li>
			</ul>
		</div>
	</div>
</div>
</body>
</html>
]])

	return table.concat(SiteContent)
end
