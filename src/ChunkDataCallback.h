
// ChunkDataCallback.h

// Declares the cChunkDataCallback interface and several trivial descendants, for reading chunk data





#pragma once

#include "ChunkData.h"





/** Interface class used for getting data out of a chunk using the GetAllData() function.
Implementation must use the pointers immediately and NOT store any of them for later use
The virtual methods are called in the same order as they're declared here. */
class cChunkDataCallback abstract
{
public:

	virtual ~cChunkDataCallback() {}

	/** Called before any other callbacks to inform of the current coords
	(only in processes where multiple chunks can be processed, such as cWorld::ForEachChunkInRect()).
	If false is returned, the chunk is skipped. */
	virtual bool Coords(int a_ChunkX, int a_ChunkZ) { UNUSED(a_ChunkX); UNUSED(a_ChunkZ); return true; }

	/** Called once to let know if the chunk lighting is valid. Return value is ignored */
	virtual void LightIsValid(bool a_IsLightValid) { UNUSED(a_IsLightValid); }

	/** Called once to export block data. */
	virtual void ChunkData(const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData) { UNUSED(a_BlockData); UNUSED(a_LightData); }

	/** Called once to provide heightmap data. */
	virtual void HeightMap(const cChunkDef::HeightMap & a_HeightMap) { UNUSED(a_HeightMap); }

	/** Called once to provide biome data. */
	virtual void BiomeMap(const cChunkDef::BiomeMap & a_BiomeMap) { UNUSED(a_BiomeMap); }

	/** Called for each entity in the chunk */
	virtual void Entity(cEntity * a_Entity) { UNUSED(a_Entity); }

	/** Called for each blockentity in the chunk */
	virtual void BlockEntity(cBlockEntity * a_Entity) { UNUSED(a_Entity); }
} ;




/** A simple implementation of the cChunkDataCallback interface that just copies the cChunkData */
class cChunkDataCopyCollector :
	public cChunkDataCallback
{
public:

	ChunkBlockData m_BlockData;
	ChunkLightData m_LightData;

private:

	virtual void ChunkData(const ChunkBlockData & a_BlockData, const ChunkLightData & a_LightData) override
	{
		m_BlockData.Assign(a_BlockData);
		m_LightData.Assign(a_LightData);
	}
};
