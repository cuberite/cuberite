
// SetChunkData.h

// Defines the SetChunkData struct that contains the data for a loaded / generated chunk, ready to be set

#pragma once

#include "ChunkData.h"
#include "BlockEntities/BlockEntity.h"





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
};
