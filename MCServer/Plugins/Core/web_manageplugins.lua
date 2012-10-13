local function Button_RemovePlugin( Name, Index )
	return "<form method='POST'><input type='hidden' name='PluginName' value='"..Name.."'><input type='hidden' name='PluginIndex' value='"..Index.."'><input type='submit' name='RemovePlugin' value='Remove'></form>"
end

local function Button_EnablePlugin( Name )
	return [[<form method="POST"><input type="hidden" name="PluginName", value="]].. Name ..[["><input type="submit" name="EnablePlugin" value="Enable"></form>]]
end

local function Button_DisablePlugin( Name )
	return [[<form method="POST"><input type="hidden" name="PluginName", value="]].. Name ..[["><input type="submit" name="DisablePlugin" value="Disable"></form>]]
end

local function FindPluginID( SettingsIni, PluginName )
	local KeyIdx = SettingsIni:FindKey("Plugins")
	local NumValues = SettingsIni:GetNumValues( KeyIdx )
	
	for i = 0, NumValues-1 do
		LOGINFO( SettingsIni:GetValue(KeyIdx, i) )
		if( SettingsIni:GetValue(KeyIdx, i) == PluginName ) then
			return i
		end
	end
	
	return nil
end

local function RemovePluginFromIni( SettingsIni, PluginName )
	local KeyIdx = SettingsIni:FindKey("Plugins")
	local PluginIdx = FindPluginID( SettingsIni, PluginName )
	
	if( PluginIdx == nil ) then
		LOGINFO("Got nil! NOOOO")
		return false
	end
	
	local Name = SettingsIni:GetValue( KeyIdx, PluginIdx )
	if( Name ~= PluginName ) then
		LOGINFO("not the same name T_T '" .. Name .. "' '" .. PluginName .. "'")
	end
	if( (Name == PluginName) and (SettingsIni:DeleteValueByID( KeyIdx, PluginIdx ) == true) ) then
		return SettingsIni:WriteFile()
	end
	
	return false
end

local function AddPluginToIni( SettingsIni, PluginName )
	RemovePluginFromIni( SettingsIni, PluginName ) -- Make sure there are no duplicates
	
	if( SettingsIni:SetValue("Plugins", "Plugin", PluginName, true ) == true ) then
		return SettingsIni:WriteFile()
	end
	
	return false
end

local function HandlePluginListChanges( Request, SettingsIni )
	local Content = ""
			
	if( Request.PostParams["EnablePlugin"] ~= nil
		and Request.PostParams["PluginName"] ~= nil ) then
		
		local PluginName = Request.PostParams["PluginName"]
	
		local PM = cRoot:Get():GetPluginManager()
		if( PM:LoadPlugin( PluginName ) == false ) then
			Content = "Could not enable '".. PluginName .."'!"
		end
	
		if( AddPluginToIni( SettingsIni, PluginName ) == true ) then
			Content = "Enabled plugin '".. PluginName .."'"
		else
			Content = "Enabled plugin '".. PluginName .."' but could not add it to settings.ini"
		end
		
	
	elseif( Request.PostParams["DisablePlugin"] ~= nil
		and Request.PostParams["PluginName"] ~= nil ) then
		
		local PluginName = Request.PostParams["PluginName"]
		
		local PM = cRoot:Get():GetPluginManager()
		PM:DisablePlugin( PluginName )
		
		if( RemovePluginFromIni( SettingsIni, PluginName ) == true ) then
			Content = "Disabled plugin '".. PluginName .."'"
		else
			Content = "Disabled plugin '".. PluginName .."' but could not remove it from settings.ini"
		end
		
		
		
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
	
	local SettingsIni = cIniFile("settings.ini")
	if( SettingsIni:ReadFile() == true ) then
		Content = Content .. HandlePluginListChanges( Request, SettingsIni )
	else
		Content = Content .. "Cannot find/modify settings.ini"
	end
	
	local PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:FindPlugins() -- Refreshes the plugin list
	local PluginList = PluginManager:GetAllPlugins()
	
	Content = Content .. "<h4>Currently installed plugins</h4>"
	Content = Content .. "<table>"
	for k, Plugin in pairs(PluginList) do
		Content = Content .. "<tr><td>".. k .."</td>"
		if( Plugin ) then 
			Content = Content .. "<td>" .. Plugin:GetName() .. " V. " .. Plugin:GetVersion() .. "</td><td>" .. Button_DisablePlugin(k) .. "</td>"
		else
			Content = Content .. "<td></td><td>" .. Button_EnablePlugin(k) .. "</td>"
		end
		Content = Content .. "</tr>"
	end
	Content = Content .. "</table>"
	
	Content = Content .. "<h4>Reload</h4>"
	Content = Content .. "<form method='POST'>"
	Content = Content .. "<p>Click the reload button to reload all plugins according to <strong>settings.ini</strong>!"
	Content = Content .. "<input type='submit' name='reload' value='Reload!'></p>"
	Content = Content .. "</form>"

	return Content
end