function HandlePluginsCommand( Split, Player )

	local PluginManager = cRoot:Get():GetPluginManager()
	local PluginList = PluginManager:GetAllPlugins()

	local PluginTable = {}
	for k, Plugin in pairs( PluginList ) do
		if Plugin then
			table.insert( PluginTable, Plugin:GetName() )
		end
	end

	SendMessage( Player, "There are " .. #PluginTable .. " loaded plugins" )
	SendMessage( Player, table.concat( PluginTable , " " ) )
	return true

end
