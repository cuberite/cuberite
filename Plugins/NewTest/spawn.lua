function HandleSpawnCommand( Split, Player )
	World = Player:GetWorld()
	Player:TeleportTo( World:GetSpawnX(), World:GetSpawnY(), World:GetSpawnZ() )
	LOGINFO( Player:GetName() .. " returned to spawn." )
	return true
end