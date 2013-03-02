function HandleSpawnCommand( Split, Player )
	World = Player:GetWorld()
	X[Player:GetName()] = Player:GetPosX()
	Y[Player:GetName()] = Player:GetPosY()
	Z[Player:GetName()] = Player:GetPosZ()
	Player:TeleportTo( World:GetSpawnX(), World:GetSpawnY(), World:GetSpawnZ() )
	LOGINFO( Player:GetName() .. " returned to spawn." )
	return true
end