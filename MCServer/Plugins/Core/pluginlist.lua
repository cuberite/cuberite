function HandlePluginListCommand( Split, Player )
	local PluginManager = cRoot:Get():GetPluginManager()
	local PluginList = PluginManager:GetAllPlugins()

	local PluginTable = {}
	for i, Plugin in ipairs( PluginList ) do
		table.insert(PluginTable, Plugin:GetName() )
	end

	Player:SendMessage( cChatColor.Green .. "Loaded plugins: (" .. #PluginTable .. ")" )
	Player:SendMessage( cChatColor.Gold .. table.concat(PluginTable, cChatColor.Gold.." ") )
	return true
end