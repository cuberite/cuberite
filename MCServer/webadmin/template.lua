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
	local Title = "MCServer"
	local MemoryUsage = WebAdmin:GetMemoryUsage()
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

<style type="text/css" media="screen">

	/* reset CSS */
	
	html, body, div, span, applet, object, iframe,
	h1, h2, h3, h4, h5, h6, p, blockquote, pre,
	a, abbr, acronym, address, big, cite, code,
	del, dfn, em, font, img, ins, kbd, q, s, samp,
	small, strike, strong, sub, sup, tt, var,
	b, u, i, center,
	dl, dt, dd, ol, ul, li,
	fieldset, form, label, legend,
	table, caption, tbody, tfoot, thead, tr, th, td {
		margin: 0;
		padding: 0;
		border: 0;
		outline: 0;
		font-size: 100%;
		vertical-align: baseline;
		background: transparent;
	}
	body {
		line-height: 1;
	}
	ol, ul {
		list-style: none;
	}
	blockquote, q {
		quotes: none;
	}

	/* remember to define focus styles! */
	:focus {
		outline: 0;
	}

	/* remove textarea resize at Safari */
	textarea { 
		resize: none;
	}

	/* remember to highlight inserts somehow! */
	ins {
		text-decoration: none;
	}
	del {
		text-decoration: line-through;
	}

	/* tables still need 'cellspacing="0"' in the markup */
	table {
		border-collapse: collapse;
		border-spacing: 0;
	}


	/*
		Origional from http://www.perspectived.com/
		Modified by Ben Phelps
		Made for FakeTruth - MCServer
	*/

	/* Basic  ---------------------------------------- */

	.clear { clear: both; }

	body {
		background: white;
		font-family: Arial, Helvetica, sans-serif;
		font-size: 12px;
		color: #646464;
		text-align: center;
	}

	#wrapper {
		text-align: left;
		width: 930px;
		margin: 0 auto;
	}

	/* Logo  ---------------------------------------- */

	h1 {
		margin: 15px 0 10px 5px;
		width: 180px;
		height: 36px;
		background: url(data:image/png;base64,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) no-repeat left top;
	}

	h1 a {
		display: block;
		width: 225px;
		height: 28px;
	}

	h1 span { display: none; }

	a {
		color: #646464;
	}

	/* Container ---------------------------------------- */

	#containerHolder {
		background: #eee;
		padding: 5px;
	}


	#container {
		background: #fff url(data:image/gif;base64,R0lGODlhtAABAIAAAN3d3f///yH5BAAAAAAALAAAAAC0AAEAAAIMjI+py+0Po5y0WgYKADs%3D) repeat-y left top;
		border: 1px solid #ddd;
		width: 918px;

	}

	#connectHolder {
		background: #eee;
		padding: 5px;
		margin-bottom:8px;
	}


	#connect {
		border: 1px solid #ddd;
		background-color: #fff;
		padding:5px;
		width: 908px;
	}

	.pics {  
	    height:  375px;
	    width:   600px;
	} 

	.pics img {  
	    padding: 5px;  
	    border:  1px solid #ddd;  
	    background-color: #eee;  
	    width:  600px; 
	    height: 375px;
		margin-left: 15px;
	}

	/* Login -------------------------------------- */

	#loginLogo {
		margin: 0 auto;
		margin-top:100px;
		width: 180px;
		height: 36px;
		background-image: url(data:image/png;base64,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);
	}

	#loginHolder {
		background: #eee;
		padding: 5px;
		width: 310px;
		margin: 0 auto;
		height: 90px;
		margin-top:20px;
	}

	#login {
		padding:10px;
		width: 288px;
		height: 68px;
		border: 1px solid #ddd;
		background:#fff;
		text-align: left;
	}


	/* Sidebar ---------------------------------------- */

	#sidebar {
		width: 179px;
		float: left;
	}

	#sidebar .sideNav { width: 179px; }

	#sidebar .sideNav li { border-bottom: 1px solid #ddd; width: 179px; }

	#sidebar .sideNav li a {
		display: block;
		color: #646464;
		background: #f6f6f6;
		text-decoration: none;
		height: 29px;
		line-height: 29px;
		padding: 0 19px;
		width: 141px;
	}

	#sidebar .sideNav li a:hover { background: #fdfcf6; }

	#sidebar .sideNav li a.active, #sidebar .sideNav li a.active:hover {
		background: #f0f7fa;
		color: #c66653;
	}

	/* Breadcrumb ---------------------------------------- */

	h2 {
		width: 718px;
		float: right;
		color: #646464;
		font-size: 16px;
		line-height: 16px;
		font-weight: bold;
		margin: 20px 0 0 0;
		padding: 0 0 10px 0;
		border-bottom: 1px solid #ddd;
	}

	h2 a {
		color: #646464;
		text-decoration: none;
	}

	h2 a.active { color: #c66653; }

	h2 a:hover { text-decoration: underline; }

	/* Content ---------------------------------------- */

	#main {
		width: 700px;
		float: right;
		padding: 0 19px 0 0;
	}

	#main p {

		padding: 10px;

	}

	h3 {
		font-size: 14px;
		line-height: 14px;
		font-weight: bold;
		color: #5494af;
		padding: 0 0 0 10px;
		margin: 20px 0 10px;
	}
	
	h4 {
		padding: 0 0 0 10px;
		margin: 20px 0 10px;
	}
	
	#main ul {
		padding: 0 0 0 10px;
		list-style-type: circle;
		list-style-position: inside;
	}

	#main table {
		border-top: 1px solid #ddd;
		width: 700px;
	}

	#main table tr th {
		text-align: left;
		background: #f6f6f6;
		padding: 0px 20px;
		height: 20px;
		line-height: 20px;
		border-bottom: 1px solid #ddd;
	}
	
	#main table tr td {
		background: #f6f6f6;
		padding: 0px 20px;
		height: 29px;
		line-height: 29px;
		border-bottom: 1px solid #ddd;
	}

	#main table tr.odd td {
		background: #fbfbfb;
	}

	#main table tr:hover td { background: #fdfcf6; }

	#main table .action {
		text-align: right;
		padding: 0 20px 0 10px;
	}

	#main table tr .action a { margin: 0 0 0 10px; text-decoration: none; color: #9b9b9b; }
	#main table tr:hover .action .edit { color: #c5a059; }
	#main table tr:hover .action .delete { color: #a02b2b; }
	#main table tr:hover .action .view { color: #55a34a; }

	#main table tr:hover .action a:hover { text-decoration: underline; }

	fieldset {
		border: 1px solid #ddd;
		padding: 19px;
		margin: 0 0 20px 0;
		background: #fbfbfb;
	}

	form p { margin: 0 0 14px 0; float: left; width: 100%;  }

	label {
		display: block;
		width: 100%;
		margin: 0 0 7px 0;
		line-height: 12px;
	}
	
	/* Footer ---------------------------------------- */

	#footer {
		margin: 10px 0 30px 0;
		font-size: 11px;
		line-height: 11px;
		color: #9B9B9B;
		padding: 0 0 0 5px;
	}

	#footer a { color: #9B9B9B; }

	#footer a:hover { text-decoration: none; }
</style>

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
	    
		<p id="footer">MCServer is using: ]] .. MemoryUsage .. [[MB of memory; Current chunk count: ]] .. NumChunks .. [[ </p>
	</div>
	<!-- // #wrapper -->
</body>
</html>
	]])
	
	return table.concat(SiteContent)
end