function HandleRequest_ManageServer( Request )
	local Content = "" 
	if( Request.PostParams["RestartServer"] ~= nil ) then
		cRoot:Get():ServerCommand("restart")
	elseif( Request.PostParams["ReloadServer"] ~= nil ) then
		cRoot:Get():GetPluginManager():ReloadPlugins()
	elseif( Request.PostParams["StopServer"] ~= nil ) then
		cRoot:Get():ServerCommand("stop")
	end
	Content = Content .. [[
	<form method="POST">]]

	Content = Content .. [[<table>
	<tr style="padding-top:5px;">
	<td><input type="submit" value="Restart Server" name="RestartServer"> <br /> restart the server</td>
	<td><input type="submit" value="Reload Server" name="ReloadServer"> <br /> reload the server</td>
	<td><input type="submit" value="Stop Server" name="StopServer"> <br /> stop the server</td>
	</tr>
	</table>
	
	]]
	return Content
end

