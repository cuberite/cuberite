function OnPlayerMoving( Player )
	if LimitWorld == true then
		local World = Player:GetWorld()
		local SpawnX = math.floor(World:GetSpawnX() / 16)
		local SpawnZ = math.floor(World:GetSpawnZ() / 16)
		local X = math.floor(Player:GetPosX() / 16)
		local Z = math.floor(Player:GetPosZ() / 16)
		if ( (SpawnX + LimitWorldWidth - 1) < X ) then 
			Player:TeleportTo(Player:GetPosX() - 1, Player:GetPosY(), Player:GetPosZ()) 
		end
		if ( (SpawnX - LimitWorldWidth + 1) > X ) then
			Player:TeleportTo(Player:GetPosX() + 1, Player:GetPosY(), Player:GetPosZ()) 
		end
		if ( (SpawnZ + LimitWorldWidth - 1) < Z ) then 
			Player:TeleportTo(Player:GetPosX(), Player:GetPosY(), Player:GetPosZ() - 1) 
		end
		if ( (SpawnZ - LimitWorldWidth + 1) > Z ) then
			Player:TeleportTo(Player:GetPosX(), Player:GetPosY(), Player:GetPosZ() + 1) 
		end
	end
end