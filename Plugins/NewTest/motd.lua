function HandleMOTDCommand( Split, Player )
	ShowMOTDTo( Player )
	return true
end

function ShowMOTDTo( Player )
	Player:SendMessage( cChatColor.Gold .. "Welcome to the MCServer test server!" );
	Player:SendMessage( cChatColor.Gold .. "http://mcserver.ae-c.net/" );
	Player:SendMessage( cChatColor.Gold .. "Type /help for all commands" );
end