function HandleMOTDCommand( Split, Player )
	ShowMOTDTo( Player )
	return true
end

function ShowMOTDTo( Player )
	Player:SendMessage( cChatColor.Gold .. "Welcome to the MCServer test server!" );
	Player:SendMessage( cChatColor.Gold .. "http://www.mcserver.org/" );
	Player:SendMessage( cChatColor.Gold .. "Type /help for all commands" );
end