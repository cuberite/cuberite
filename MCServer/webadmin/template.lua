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





function GetDefaultPage()
	local PM = cRoot:Get():GetPluginManager()

	local SubTitle = "Current Game"
	local Content = ""
	
	Content = Content .. "<h4>Server Name:</h4>"
	Content = Content .. "<p>" .. cRoot:Get():GetServer():GetServerID() .. "</p>"
	
	Content = Content .. "<h4>Plugins:</h4><ul>"
	local AllPlugins = PM:GetAllPlugins()
	for key,value in pairs(AllPlugins) do
		if( value ~= nil and value ~= false ) then
			Content = Content ..  "<li>" .. key .. " V." .. value:GetVersion() .. "</li>"
		end
	end
	
	Content = Content .. "</ul>"
	Content = Content .. "<h4>Players:</h4><ul>"
	
	local AddPlayerToTable = function( Player )
		Content = Content .. "<li>" .. Player:GetName() .. "</li>"
	end
	cRoot:Get():ForEachPlayer( AddPlayerToTable )
	
	Content = Content .. "</ul><br>";

	return Content, SubTitle
end





function ShowPage(WebAdmin, TemplateRequest)
	SiteContent = {}
	local BaseURL = WebAdmin:GetBaseURL(TemplateRequest.Request.Path)
	local Title = "MCServer WebAdmin"
	local MemoryUsageKiB = cRoot:GetPhysicalRAMUsage()
	local NumChunks = cRoot:Get():GetTotalChunkCount()
	local PluginPage = WebAdmin:GetPage(TemplateRequest.Request)
	local PageContent = PluginPage.Content
	local SubTitle = PluginPage.PluginName
	if (PluginPage.TabName ~= "") then
		SubTitle = PluginPage.PluginName .. " - " .. PluginPage.TabName
	end
	if (PageContent == "") then
		PageContent, SubTitle = GetDefaultPage()
	end
	
	local reqParamsClass = ""
	
	for key,value in pairs(TemplateRequest.Request.Params) do
		reqParamsClass = reqParamsClass .. " param-" .. string.lower(string.gsub(key, "[^a-zA-Z0-9]+", "-") .. "-" .. string.gsub(value, "[^a-zA-Z0-9]+", "-"))
	end
	
	if (string.gsub(reqParamsClass, "%s", "") == "") then
		reqParamsClass = " no-param"
	end
	
	Output([[
<!DOCTYPE html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="icon" href="/favicon.ico">
<title>]] .. Title .. [[</title>
<link href='http://fonts.googleapis.com/css?family=Open+Sans:400,600,300' rel='stylesheet' type='text/css'>
<link rel="stylesheet" type="text/css" href="/style.css">
</head>
<body>
<div id="wrapper">
	<div id="containerHolder">
		<a href="./" class="title light">MCServer</a>
		<div id="container">
			<div id="sidebar">
				<ul class="sideNav">
					<li class='link'><a href=']] .. BaseURL .. [['>Home</a></li>
	]])


	local AllPlugins = WebAdmin:GetPlugins()
	for key,value in pairs(AllPlugins) do
		local PluginWebTitle = value:GetWebTitle()
		local TabNames = value:GetTabNames()
		if (GetTableSize(TabNames) > 0) then
			Output("<li>"..PluginWebTitle.."</li>\n");
			
			for webname,prettyname in pairs(TabNames) do
				Output("<li class='link'><a href='" .. BaseURL .. PluginWebTitle .. "/" .. webname .. "'>" .. prettyname .. "</a></li>\n")
			end
		end
	end

	
	Output([[
					</ul>
			</div>
			
			<div id="main" class="page-]] .. string.lower(PluginPage.PluginName .. "-" .. string.gsub(PluginPage.TabName, "[^a-zA-Z0-9]+", "-")) .. reqParamsClass .. [[">
				<h2 class="welcome-msg">Welcome <span class="username">]] .. TemplateRequest.Request.Username .. [[</span></h2>
				
				<hr/>
				
				<h3>]] .. SubTitle .. [[</h3>
					]] .. PageContent .. [[</div>
			<div class="clear"></div>
		</div>
	</div>
	<div id="footer"><div class="fleft">running MCServer using <span class="bold">]] .. MemoryUsageKiB / 1024 .. [[MB</span> of memory; <span class="bold">]] .. NumChunks .. [[</span> chunks</div><div class="fright">design by <a href="//www.github.com/WebFreak001">WebFreak001</a></div><div class="clear"></div></div>
</div>
</body>
</html>
]])
	
	return table.concat(SiteContent)
end
