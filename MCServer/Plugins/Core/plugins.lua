function HandlePluginsCommand( Split, Player )
	local PluginManager = cRoot:Get():GetPluginManager()
	local PluginList = PluginManager:GetAllPlugins()

	local PluginTable = {}
	for k, Plugin in pairs( PluginList ) do
		if ( Plugin ) then
			table.insert(PluginTable, Plugin:GetName() )
		end
	end

	Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "There are " .. #PluginTable .. " loaded plugins")
	Player:SendMessage(cChatColor.Gold .. table.concat(PluginTable, cChatColor.Gold.." ") )
	return true
end