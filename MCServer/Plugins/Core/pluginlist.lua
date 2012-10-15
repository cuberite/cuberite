function HandlePluginListCommand( Split, Player )
	local PluginManager = cRoot:Get():GetPluginManager()
	local PluginList = PluginManager:GetAllPlugins()

	local PluginTable = {}
	for k, Plugin in pairs( PluginList ) do
		if ( Plugin ) then
			table.insert(PluginTable, Plugin:GetName() )
		end
	end

	Player:SendMessage( cChatColor.Green .. "Loaded plugins: (" .. #PluginTable .. ")" )
	Player:SendMessage( cChatColor.Gold .. table.concat(PluginTable, cChatColor.Gold.." ") )
	return true
end