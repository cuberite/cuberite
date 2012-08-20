local function Button_RemovePlugin( Name, Index )
	return "<form method='POST'><input type='hidden' name='PluginName' value='"..Name.."'><input type='hidden' name='PluginIndex' value='"..Index.."'><input type='submit' name='RemovePlugin' value='Remove'></form>"
end

local function HandlePluginListChanges( Request, SettingsIni )
	local Content = ""
	if( Request.PostParams["RemovePlugin"] ~= nil
		and Request.PostParams["PluginName"] ~= nil
		and Request.PostParams["PluginIndex"] ~= nil ) then -- Removing a plugin
		
		local KeyIdx = SettingsIni:FindKey("Plugins")
		local PluginIdx = Request.PostParams["PluginIndex"]
		
		local PluginName = SettingsIni:GetValue( KeyIdx, PluginIdx )
		if( (PluginName == Request.PostParams["PluginName"]) and (SettingsIni:DeleteValueByID( KeyIdx, PluginIdx ) == true) ) then
			SettingsIni:WriteFile()
			Content = "Removed plugin '" .. PluginName .. "'"
		else
			Content = "Whoops! Something went wrong!"
		end
		
		
	elseif( Request.PostParams["AddPlugin"] ~= nil 
		and Request.PostParams["PluginName"] ~= nil ) then -- Add a plugin
		
		SettingsIni:SetValue("Plugins", "NewPlugin", Request.PostParams["PluginName"], true )
		SettingsIni:WriteFile()
		
		Content = "Added plugin '".. Request.PostParams["PluginName"] .."'"
		
	end
	
	if( #Content > 0 ) then
		return "<p><font color='red'><strong>INFO: " .. Content .. "</strong></font></p>"
	else
		return ""
	end
end

function HandleRequest_ManagePlugins( Request )
	local Content = ""
	
	if( Request.PostParams["reload"] ~= nil ) then
		Content = Content .. "<head><meta http-equiv=\"refresh\" content=\"2;\"></head>"
		Content = Content .. "<p>Reloading plugins... This can take a while depending on the plugins you're using.</p>"
		cRoot:Get():GetPluginManager():ReloadPlugins()
		return Content
	end
	
	local PluginManager = cRoot:Get():GetPluginManager()
	local PluginList = PluginManager:GetAllPlugins()
	
	Content = Content .. "<h4>Currently active plugins</h4>"
	Content = Content .. "<table>"
	for k, Plugin in pairs(PluginList) do
		Content = Content .. "<tr><td>" .. Plugin:GetName() .. " V. " .. Plugin:GetVersion() .. "</td></tr>"
	end
	Content = Content .. "</table>"
	
	local SettingsIni = cIniFile("settings.ini")
	if( SettingsIni:ReadFile() == true ) then
		Content = Content .. "<h4>Plugins according to settings.ini</h4>"
		
		Content = Content .. HandlePluginListChanges( Request, SettingsIni )
		
		Content = Content .. "<table>"
		
		local KeyIdx = SettingsIni:FindKey("Plugins")
		local NumValues = SettingsIni:GetNumValues( KeyIdx )
		for i = 0, NumValues-1 do
			local ValueName = SettingsIni:GetValueName(KeyIdx, i )
			local PluginName = SettingsIni:GetValue(KeyIdx, i)
			Content = Content .. "<tr>"
			Content = Content .. "<td>" .. ValueName .. ": " .. PluginName .. "</td>"
			Content = Content .. "<td>" .. Button_RemovePlugin( PluginName, i ) .. "</td>"
			Content = Content .. "</tr>"
		end
		Content = Content .. "</table>"
	end
	
	Content = Content .. "<h4>Add plugin to settings.ini</h4>"
	Content = Content .. "<form method='POST'>"
	Content = Content .. "<input type='text' name='PluginName'><input type='submit' name='AddPlugin' value='Add Plugin'>"
	Content = Content .. "</form>"
	
	Content = Content .. "<h4>Reload</h4>"
	Content = Content .. "<form method='POST'>"
	Content = Content .. "<p>Click the reload button to reload all plugins!<br>"
	Content = Content .. "<input type='submit' name='reload' value='Reload!'></p>"
	Content = Content .. "</form>"

	return Content
end