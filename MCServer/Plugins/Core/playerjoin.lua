function OnPlayerJoined(Player)
	ShowMOTDTo( Player )
	AddMessage( Player:GetName() .. " has joined the game", " " )
	return false
end