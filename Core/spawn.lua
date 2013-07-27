function HandleSpawnCommand(Split, Player)
	World = Player:GetWorld()
	SetBackCoordinates(Player)
	Player:TeleportToCoords(World:GetSpawnX(), World:GetSpawnY(), World:GetSpawnZ())
	LOGINFO(Player:GetName() .. " returned to spawn.")
	return true
end