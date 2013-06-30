function OnChunkGenerating(World, ChunkX, ChunkZ, ChunkDesc)
	if (LimitWorld == true) then
		if not LimitWorldsCuboid[World:GetName()]:IsInside( Vector3i(ChunkX, 128, ChunkZ) ) then
			FillBlocks(ChunkDesc)
		end
	end
end

function FillBlocks(ChunkDesc)
	ChunkDesc:FillBlocks(0,0)
	ChunkDesc:SetUseDefaultBiomes(false)
	ChunkDesc:SetUseDefaultHeight(false)
	ChunkDesc:SetUseDefaultComposition(false)
	ChunkDesc:SetUseDefaultStructures(false)
	ChunkDesc:SetUseDefaultFinish(false)
end