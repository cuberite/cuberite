function HandleReloadCommand( Split, Player )
	Server = cRoot:Get():GetServer()
	Server:SendMessage( cChatColor.Green .. "Reloading all plugins." )
	cRoot:Get():GetPluginManager():ReloadPlugins()
	return true
end