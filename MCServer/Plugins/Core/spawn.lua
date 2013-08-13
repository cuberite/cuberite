function HandleSpawnCommand(Split, Player)

	World = Player:GetWorld()
	SetBackCoordinates(Player)
	Player:TeleportToCoords(World:GetSpawnX(), World:GetSpawnY(), World:GetSpawnZ())
	SendMessageSuccess( Player, "Returned to world spawn" )
	return true

end
