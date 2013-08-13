




function OnPlayerJoined(Player)
	ShowMOTDTo(Player)
	Player:GetWorld():BroadcastChat(cChatColor.Yellow .. "[JOIN] " .. cChatColor.White .. Player:GetName() .. " has joined the game")
	return false
end





function OnDisconnect(Player, Reason)
	Player:GetWorld():BroadcastChat(cChatColor.Yellow .. "[LEAVE] " .. cChatColor.White .. Player:GetName() .. " has left the game")
	return true
end




