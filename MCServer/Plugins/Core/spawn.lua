function HandleSpawnCommand(Split, Player)
	World = Player:GetWorld()
	SetBackCoordinates(Player)
	Player:TeleportToCoords(World:GetSpawnX(), World:GetSpawnY(), World:GetSpawnZ())
    Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Returned to world spawn" )
	return true
end