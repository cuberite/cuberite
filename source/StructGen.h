
// StructGen.h

/* Interfaces to the various structure generators:
	- cStructGenTrees
	- cStructGenMarbleCaves
	- cStructGenOres
*/





#pragma once

#include "cChunkGenerator.h"
#include "cNoise.h"





class cStructGenTrees :
	public cStructureGen
{
public:
	cStructGenTrees(int a_Seed, cBiomeGen * a_BiomeGen, cTerrainHeightGen * a_HeightGen, cTerrainCompositionGen * a_CompositionGen) :
		m_Seed(a_Seed),
		m_Noise(a_Seed),
		m_BiomeGen(a_BiomeGen),
		m_HeightGen(a_HeightGen),
		m_CompositionGen(a_CompositionGen)
	{}
	
protected:

	int m_Seed;
	cNoise m_Noise;
	cBiomeGen *              m_BiomeGen;
	cTerrainHeightGen *      m_HeightGen;
	cTerrainCompositionGen * m_CompositionGen;
	
	void GenerateSingleTree(
		int a_ChunkX, int a_ChunkZ, int a_Seq,
		cChunkDef::BlockTypes & a_BlockTypes,
		cChunkDef::BlockNibbles & a_BlockMetas,
		const cChunkDef::HeightMap & a_Height,
		const cChunkDef::BiomeMap & a_Biomes,
		sSetBlockVector & a_Blocks
	) ;
	
	int GetNumTrees(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BiomeMap & a_Biomes
	);
	
	// cStructureGen override:
	virtual void GenStructures(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMetas,  // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
		cEntityList & a_Entities,                // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
	) override;
} ;





class cStructGenMarbleCaves :
	public cStructureGen
{
public:
	cStructGenMarbleCaves(int a_Seed) : m_Seed(a_Seed) {}
	
protected:

	int m_Seed;
	
	// cStructureGen override:
	virtual void GenStructures(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
		cEntityList & a_Entities,               // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
	) override;
} ;





class cStructGenOreNests :
	public cStructureGen
{
public:
	cStructGenOreNests(int a_Seed) : m_Noise(a_Seed), m_Seed(a_Seed) {}
	
protected:
	cNoise m_Noise;
	int    m_Seed;
	
	// cStructureGen override:
	virtual void GenStructures(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
		cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
		cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
		cEntityList & a_Entities,               // Entities may be added or deleted
		cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
	) override;
	
	void GenerateOre(int a_ChunkX, int a_ChunkZ, BLOCKTYPE a_OreType, int a_MaxHeight, int a_NumNests, int a_NestSize, cChunkDef::BlockTypes & a_BlockTypes, int a_Seq);
} ;




