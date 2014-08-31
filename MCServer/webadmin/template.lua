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
<link rel="icon" href="data:application/octet-stream;base64,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">
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
			Output("<li>"..PluginWebTitle.."</li>");
			
			for webname,prettyname in pairs(TabNames) do
				Output("<li><a href='" .. BaseURL .. PluginWebTitle .. "/" .. webname .. "'>" .. prettyname .. "</a></li>")
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
