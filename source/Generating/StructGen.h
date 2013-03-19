
// StructGen.h

/* Interfaces to the various structure generators:
	- cStructGenTrees
	- cStructGenMarbleCaves
	- cStructGenOres
*/





#pragma once

#include "ComposableGenerator.h"
#include "../Noise.h"





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
	
	/** Generates and applies an image of a single tree.
	Parts of the tree inside the chunk are applied to a_BlockX.
	Parts of the tree outside the chunk are stored in a_OutsideX
	*/
	void GenerateSingleTree(
		int a_ChunkX, int a_ChunkZ, int a_Seq,
		cChunkDesc & a_ChunkDesc,
		sSetBlockVector & a_OutsideLogs,
		sSetBlockVector & a_OutsideOther
	) ;
	
	/// Applies an image into chunk blockdata; all blocks outside the chunk will be appended to a_Overflow
	void ApplyTreeImage(
		int a_ChunkX, int a_ChunkZ,
		cChunkDesc & a_ChunkDesc,
		const sSetBlockVector & a_Image,
		sSetBlockVector & a_Overflow
	);

	int GetNumTrees(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BiomeMap & a_Biomes
	);
	
	// cStructureGen override:
	virtual void GenStructures(cChunkDesc & a_ChunkDesc) override;
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
	virtual void GenStructures(cChunkDesc & a_ChunkDesc) override;
	
	void GenerateOre(int a_ChunkX, int a_ChunkZ, BLOCKTYPE a_OreType, int a_MaxHeight, int a_NumNests, int a_NestSize, cChunkDef::BlockTypes & a_BlockTypes, int a_Seq);
} ;





class cStructGenLakes :
	public cStructureGen
{
public:
	cStructGenLakes(int a_Seed, BLOCKTYPE a_Fluid, cTerrainHeightGen & a_HeiGen) :
		m_Noise(a_Seed),
		m_Seed(a_Seed),
		m_Fluid(a_Fluid),
		m_HeiGen(a_HeiGen)
	{
	}
	
protected:
	cNoise              m_Noise;
	int                 m_Seed;
	BLOCKTYPE           m_Fluid;
	cTerrainHeightGen & m_HeiGen;
	
	// cStructureGen override:
	virtual void GenStructures(cChunkDesc & a_ChunkDesc) override;
	
	/// Creates a lake image for the specified chunk into a_Lake
	void CreateLakeImage(int a_ChunkX, int a_ChunkZ, cBlockArea & a_Lake);
} ;





