function HandleSaveAllCommand( Split, Player )
	cRoot:Get():SaveAllChunks();
    local Server = cRoot:Get():GetServer()
	Server:SendMessage(cChatColor.Rose .. "[WARNING] " .. cChatColor.White .. "Saving all worlds!")
	return true;
end

function HandleStopCommand( Split, Player )
    Server = cRoot:Get():GetServer()
    local Server = cRoot:Get():GetServer()
	Server:SendMessage(cChatColor.Red .. "[WARNING] " .. cChatColor.White .. "Server is terminating!" )
	cRoot:Get():QueueExecuteConsoleCommand("stop")
	return true
end

function HandleReloadCommand( Split, Player )
    Server = cRoot:Get():GetServer()
    local Server = cRoot:Get():GetServer()
	Server:SendMessage(cChatColor.Rose .. "[WARNING] " .. cChatColor.White .. "Reloading all plugins!" )
	cRoot:Get():GetPluginManager():ReloadPlugins()
	return true
end