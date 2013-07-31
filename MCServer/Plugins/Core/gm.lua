function HandleChangeGMCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /gm [GameMode (0|1)]" )
		return true
	end
	Player:SetGameMode(Split[2])
	return true
end