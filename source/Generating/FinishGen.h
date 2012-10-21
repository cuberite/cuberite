
// FinishGen.h

/* Interfaces to the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
	- cFinishGenLilypads
	- cFinishGenBottomLava
*/





#include "ChunkGenerator.h"
#include "../Noise.h"





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
	
	/// Tries to place sugarcane at the coords specified, returns true if successful
	bool TryAddSugarcane(
		int a_ChunkX, int a_ChunkZ,
		int a_RelX, int a_RelY, int a_RelZ,      // relative block coords of the sugarcane's base
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta    // Block meta to read and change
	);
	
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





class cFinishGenLilypads :
	public cFinishGen
{
public:
	cFinishGenLilypads(int a_Seed) :
		m_Noise(a_Seed)
	{
	}
	
protected:
	cNoise m_Noise;
	
	int GetNumLilypads(const cChunkDef::BiomeMap & a_BiomeMap);
	
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





class cFinishGenBottomLava :
	public cFinishGen
{
public:
	cFinishGenBottomLava(int a_Level) :
		m_Level(a_Level)
	{
	}
	
protected:
	int m_Level;
	
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






class cFinishGenPreSimulator :
	public cFinishGen
{
public:
	cFinishGenPreSimulator(void);
	
protected:
	// Drops hanging sand and gravel down to the ground, recalculates heightmap
	void CollapseSandGravel(
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::HeightMap & a_HeightMap       // Height map to update by the current data
	);
	
	/** For each fluid block:
	- if all surroundings are of the same fluid, makes it stationary; otherwise makes it flowing (excl. top)
	- all fluid on the chunk's edge is made flowing
	*/
	void StationarizeFluid(
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::HeightMap & a_HeightMap,      // Height map to read
		BLOCKTYPE a_Fluid,
		BLOCKTYPE a_StationaryFluid
	);

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




