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

	local SubTitle = "Home"
	local Content = ""
	
	Content = Content .. "<h4>Home Home Home</h4><ul>I have no idea what this content should do.</ul>"
	
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
	<title>]] .. Title .. [[</title>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<link href="http://fonts.googleapis.com/css?family=Open+Sans:400,600,300" rel="stylesheet" type="text/css">
	<link rel="stylesheet" type="text/css" href="/style.css">
	<link rel="icon" href="/favicon.ico">
</head>

<body>
	<div class="contention">
		<div class="pagehead">
			<div class="row1">
				<div class="wrapper">
					<img src="/logo_login.png" alt="MCServer Logo" class="logo">
				</div>
			</div>
			<div id="panel">
				<div class="upper">
					<div class="wrapper">
						<ul class="menu top_links">
							<li><a href=".././"><img src="/log_out.png" style="vertical-align:bottom;"> Log Out</a></li>
						</ul>
						<div class="welcome"><strong>Welcome back, ]] .. TemplateRequest.Request.Username .. [[</strong></div>
					</div>
				</div>
			</div>
		</div>
		<div class="row2 push10">
			<div class="wrapper padtopp">
			<table width="100%" border="0" align="center"><tbody>
				<tr>
					<!-- start: usercp_nav -->
					<td width="180" valign="top">
						<table border="0" cellspacing="0" cellpadding="5" class="tborder">
							<tbody><tr>
								<td class="thead"><strong>Menu</strong></td>
							</tr>
							<tr>
								<td class="trow1 smalltext"><a href=']] .. BaseURL .. [[' class='usercp_nav_item usercp_nav_home'>Home</a></td>
							</tr>
						<!-- start: usercp_nav_messenger -->
						<tr><td class="tcat"><div><span class="smalltext"><strong>Server Management</strong></span></div></td></tr></tbody><tbody style="" id="usercppms_e"><tr><td class="trow1 smalltext">
	]])


	local AllPlugins = WebAdmin:GetPlugins()
	for key,value in pairs(AllPlugins) do
		local PluginWebTitle = value:GetWebTitle()
		local TabNames = value:GetTabNames()
		if (GetTableSize(TabNames) > 0) then
			Output("<div><a class='usercp_nav_item usercp_nav_pmfolder' style='text-decoration:none;'><b>"..PluginWebTitle.."</b></a></div>\n");
			
			for webname,prettyname in pairs(TabNames) do
				Output("<div><a href='" .. BaseURL .. PluginWebTitle .. "/" .. webname .. "' class='usercp_nav_item usercp_nav_sub_pmfolder'>" .. prettyname .. "</a></div>\n")
			end

			Output("<br>\n");
		end
	end

	
	Output([[</td></tr></tbody>
						</table>
					</td>
					<!-- end: usercp_nav -->
				<td valign="top" style='padding-left:25px;'>
				<table border="0" cellspacing="0" cellpadding="5" class="tborder">
				<tbody><tr>
				<td class="thead" colspan="2"><strong>]] .. SubTitle .. [[</strong></td>
				</tr>
				<tr>
				<td class="trow2">
				]] .. PageContent .. [[
				</td>
				</tr>
				</tbody></table>
				</td>
				</tr>
			</tbody></table>
			</div>
		</div>
	</div>
	<div id="footer">
		<div class="upper">
			<div class="wrapper">
				<ul class="menu bottom_links">
					<li><a>Server Name: <strong>]] .. cRoot:Get():GetServer():GetServerID() .. [[</strong></a></li>
					<li><a>Memory: <strong>]] .. MemoryUsageKiB / 1024 .. [[MB</strong></a></li>
					<li><a>Chunks: <strong>]] .. NumChunks .. [[</strong></a></li>
					<li><a><script type="text/javascript">
document.write ('Time: <strong><span id="date-time">', new Date().toLocaleString(), '<\/span><\/strong>')
if (document.getElementById) onload = function () {
	setInterval ("document.getElementById ('date-time').firstChild.data = new Date().toLocaleString()", 50)
}
</script></a></li>
				</ul>
				<ul class="menu bottom_right">
					
				</ul>
			</div>
		</div>
		<div class="lower">
			<div class="wrapper">
				<span id="current_time"><strong>FusionCare</strong> Web Developement</span>
				<span id="copyright">
					Copyright © <a href="http://www.mc-server.org" target="_blank">MCServer Team</a> 2014.
				</span>
			</div>
		</div>
	</div>
</body>

</html>
]])
	
	return table.concat(SiteContent)
end
