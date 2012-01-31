function HandleRequest_Reload( Request )
	local Content = ""
	
	if( Request.PostParams["reload"] ~= nil ) then
		Content = Content .. "<head><meta http-equiv=\"refresh\" content=\"2;././\"></head>"
		Content = Content .. "<p>Reloading plugins... This can take a while depending on the plugins you're using.</p>"
		cRoot:Get():GetPluginManager():ReloadPlugins()
	else
		Content = Content .. "<form method=POST>"
		Content = Content .. "<p>Click the reload button to reload all plugins!<br>"
		Content = Content .. "<input type=\"submit\" name=\"reload\" value=\"Reload!\"></p>"
		Content = Content .. "</form>"
	end
	return Content
end