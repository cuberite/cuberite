function HandleStopCommand( Split, Player )
	Server = cRoot:Get():GetServer()
	Server:SendMessage( cChatColor.Green .. "Stopping the server..." )
	cRoot:Get():QueueExecuteConsoleCommand("stop")
	return true
end