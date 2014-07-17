
// ChunkDataCallback.h

// Declares the cChunkDataCallback interface and several trivial descendants, for reading chunk data





#pragma once

#include "ChunkData.h"





/** Interface class used for getting data out of a chunk using the GetAllData() function.
Implementation must use the pointers immediately and NOT store any of them for later use
The virtual methods are called in the same order as they're declared here.
*/
class cChunkDataCallback abstract
{
public:

	virtual ~cChunkDataCallback() {}

	/** Called before any other callbacks to inform of the current coords
	(only in processes where multiple chunks can be processed, such as cWorld::ForEachChunkInRect()).
	If false is returned, the chunk is skipped.
	*/
	virtual bool Coords(int a_ChunkX, int a_ChunkZ) { UNUSED(a_ChunkX); UNUSED(a_ChunkZ); return true; };
	
	/// Called once to provide heightmap data
	virtual void HeightMap(const cChunkDef::HeightMap * a_HeightMap) {UNUSED(a_HeightMap); };
	
	/// Called once to provide biome data
	virtual void BiomeData(const cChunkDef::BiomeMap * a_BiomeMap) {UNUSED(a_BiomeMap); };
	
	/// Called once to let know if the chunk lighting is valid. Return value is ignored
	virtual void LightIsValid(bool a_IsLightValid) {UNUSED(a_IsLightValid); };
	
	/// Called once to export block info
	virtual void ChunkData(const cChunkData & a_Buffer) {UNUSED(a_Buffer); };
	
	/// Called for each entity in the chunk
	virtual void Entity(cEntity * a_Entity) {UNUSED(a_Entity); };
	
	/// Called for each blockentity in the chunk
	virtual void BlockEntity(cBlockEntity * a_Entity) {UNUSED(a_Entity); };
} ;





/** A simple implementation of the cChunkDataCallback interface that collects all block data into a buffer
*/
class cChunkDataCollector :
	public cChunkDataCallback
{
public:

	cChunkData m_BlockData;

protected:

	virtual void ChunkData(const cChunkData & a_BlockData) override
	{
		m_BlockData = a_BlockData.Copy();
	}
};





/** A simple implementation of the cChunkDataCallback interface that collects all block data into a single buffer
*/
class cChunkDataArrayCollector :
	public cChunkDataCallback
{
public:

	// Must be unsigned char instead of BLOCKTYPE or NIBBLETYPE, because it houses both.
	unsigned char m_BlockData[cChunkDef::BlockDataSize];

protected:

	virtual void ChunkData(const cChunkData & a_ChunkBuffer) override
	{
		a_ChunkBuffer.CopyBlockTypes(m_BlockData);
		a_ChunkBuffer.CopyMetas(m_BlockData + cChunkDef::NumBlocks);
		a_ChunkBuffer.CopyBlockLight(m_BlockData + 3 * cChunkDef::NumBlocks / 2);
		a_ChunkBuffer.CopySkyLight(m_BlockData + 2 * cChunkDef::NumBlocks);
	}
};





/** A simple implementation of the cChunkDataCallback interface that collects all block data into separate buffers */
class cChunkDataSeparateCollector :
	public cChunkDataCallback
{
public:

	cChunkDef::BlockTypes m_BlockTypes;
	cChunkDef::BlockNibbles m_BlockMetas;
	cChunkDef::BlockNibbles m_BlockLight;
	cChunkDef::BlockNibbles m_BlockSkyLight;

protected:

	virtual void ChunkData(const cChunkData & a_ChunkBuffer) override
	{
		a_ChunkBuffer.CopyBlockTypes(m_BlockTypes);
		a_ChunkBuffer.CopyMetas(m_BlockMetas);
		a_ChunkBuffer.CopyBlockLight(m_BlockLight);
		a_ChunkBuffer.CopySkyLight(m_BlockSkyLight);
	}
} ;




