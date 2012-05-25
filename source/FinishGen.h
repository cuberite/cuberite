
// FinishGen.h

/* Interfaces to the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
*/





#include "cChunkGenerator.h"





class cFinishGenSnow :
	public cFinishGen
{
protected:
	// cFinishGen override:
	virtual void GenFinish(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
		const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
		cEntityList & a_Entities,                // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
	) override;
} ;





class cFinishGenIce :
	public cFinishGen
{
protected:
	// cFinishGen override:
	virtual void GenFinish(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
		const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
		cEntityList & a_Entities,                // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
	) override;
} ;




class cFinishGenSprinkleFoliage :
	public cFinishGen
{
public:
	cFinishGenSprinkleFoliage(int a_Seed) : m_Seed(a_Seed) {}
	
protected:
	int m_Seed;

	// cFinishGen override:
	virtual void GenFinish(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
		const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
		cEntityList & a_Entities,                // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
	) override;
} ;




