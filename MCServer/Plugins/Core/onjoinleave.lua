function OnPlayerJoined(Player)
	ShowMOTDTo( Player )
    local Server = cRoot:Get():GetServer()
	Server:SendMessage(cChatColor.Yellow .. "[JOIN] " .. cChatColor.White .. Player:GetName() .. " has joined the game" )
	return false
end

function OnDisconnect(Player, Reason)
    local Server = cRoot:Get():GetServer()
    Server:SendMessage(cChatColor.Yellow .. "[LEAVE] " .. cChatColor.White .. Player:GetName() .. " has left the game" )
end