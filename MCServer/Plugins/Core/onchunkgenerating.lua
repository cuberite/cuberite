function OnChunkGenerating(World, ChunkX, ChunkZ, ChunkDesc)
	if LimitWorld == true then
		SpawnX = math.floor(World:GetSpawnX() / 16)
		SpawnZ = math.floor(World:GetSpawnZ() / 16)
		if ( (SpawnX + LimitWorldWidth) < ChunkX ) or ( (SpawnX - LimitWorldWidth) > ChunkX ) then
			FillBlocks(World, ChunkX, ChunkZ, ChunkDesc)
		end
		if ( (SpawnZ + LimitWorldWidth) < ChunkZ ) or ( (SpawnZ - LimitWorldWidth) > ChunkZ ) then
			FillBlocks(World, ChunkX, ChunkZ, ChunkDesc)
		end
	end
end

function FillBlocks(World, ChunkX, ChunkZ, ChunkDesc)
	ChunkDesc:FillBlocks(0,0)
	ChunkDesc:SetUseDefaultBiomes(false)
	ChunkDesc:SetUseDefaultHeight(false)
	ChunkDesc:SetUseDefaultComposition(false)
	ChunkDesc:SetUseDefaultStructures(false)
	ChunkDesc:SetUseDefaultFinish(false)
end