function HandleStopCommand( Split, Player )
	Server = cRoot:Get():GetServer()
	PluginManager = cRoot:Get():GetPluginManager()
	Server:SendMessage( cChatColor.Green .. "Stopping the server..." )
	PluginManager:ExecuteConsoleCommand("stop")
	--cRoot:Get():ServerCommand("stop")
	return true
end