#pragma once

#include "ChunkData.h"
#include "BlockEntities/BlockEntity.h"





/** Contains the data for a loaded / generated chunk, ready to be set into a cWorld. */
struct SetChunkData
{
	/** Initialise the structure with chunk coordinates.
	The caller is responsible for initialising the remaining members. */
	SetChunkData(const cChunkCoords a_Chunk) :
		Chunk(a_Chunk)
	{
	}

	cChunkCoords Chunk;

	ChunkBlockData BlockData;

	ChunkLightData LightData;

	cChunkDef::BiomeMap BiomeMap;
	cChunkDef::HeightMap HeightMap;

	cEntityList Entities;
	cBlockEntities BlockEntities;

	bool IsLightValid;


	/** Recalculates the HeightMap based on BlockData contents. */
	void UpdateHeightMap();
};
