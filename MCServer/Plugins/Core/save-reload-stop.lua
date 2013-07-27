function HandleSaveAllCommand( Split, Player )
	cRoot:Get():SaveAllChunks();
	Player:SendMessage(cChatColor.Green .. "All the worlds are saved")
	return true;
end

function HandleStopCommand( Split, Player )
    Server = cRoot:Get():GetServer()
	Server:SendMessage( cChatColor.Green .. "Stopping the server..." )
	cRoot:Get():QueueExecuteConsoleCommand("stop")
	return true
end

function HandleReloadCommand( Split, Player )
    Server = cRoot:Get():GetServer()
	Server:SendMessage( cChatColor.Green .. "Reloading all plugins." )
	cRoot:Get():GetPluginManager():ReloadPlugins()
	return true
end