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
	
	Output([[
<!DOCTYPE html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="icon" href="/favicon.ico">
<title>]] .. Title .. [[</title>
<link rel="stylesheet" type="text/css" media="screen" href="/style.css">
</head>

<body>
	<div id="wrapper">
		<!-- h1 tag stays for the logo, you can use the a tag for linking the index page -->
		<h1>
			<a href="]] .. BaseURL .. [["><span>MCServer</span></a>
		</h1>
		<div id="containerHolder">
			<div id="container">
				<div id="sidebar">
					<ul class="sideNav">
	]])


	local AllPlugins = WebAdmin:GetPlugins()
	for key,value in pairs(AllPlugins) do
		local PluginWebTitle = value:GetWebTitle()
		local TabNames = value:GetTabNames()
		if (GetTableSize(TabNames) > 0) then
			Output("<li>"..PluginWebTitle.."</li>\n");
			
			for webname,prettyname in pairs(TabNames) do
				Output("<li><a href='" .. BaseURL .. PluginWebTitle .. "/" .. webname .. "'>" .. prettyname .. "</a></li>\n")
			end
		end
	end

	
	Output([[
					</ul>
					<!-- // .sideNav -->
				</div>    
				<!-- // #sidebar -->
				<!-- h2 stays for breadcrumbs -->
				<h2>Welcome ]] .. TemplateRequest.Request.Username .. [[</h2>
				<div id="main">
					<h3>]] .. SubTitle .. [[</h3>
					]] .. PageContent .. [[
				</div>
				<!-- // #main -->
				
				<div class="clear"></div>
				
			</div>
			<!-- // #container -->
		</div>	
		<!-- // #containerHolder -->
	    
		<p id="footer">MCServer is using: ]] .. MemoryUsageKiB / 1024 .. [[ MiB of memory; Current chunk count: ]] .. NumChunks .. [[ </p>
	</div>
	<!-- // #wrapper -->
</body>
</html>
	]])
	
	return table.concat(SiteContent)
end
