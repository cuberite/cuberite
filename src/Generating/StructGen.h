
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
	public cFinishGen
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
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cStructGenOreNests :
	public cFinishGen
{
public:
	cStructGenOreNests(int a_Seed) : m_Noise(a_Seed), m_Seed(a_Seed) {}
	
protected:
	cNoise m_Noise;
	int    m_Seed;
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
	
	void GenerateOre(int a_ChunkX, int a_ChunkZ, BLOCKTYPE a_OreType, int a_MaxHeight, int a_NumNests, int a_NestSize, cChunkDef::BlockTypes & a_BlockTypes, int a_Seq);
} ;





class cStructGenLakes :
	public cFinishGen
{
public:
	cStructGenLakes(int a_Seed, BLOCKTYPE a_Fluid, cTerrainHeightGen & a_HeiGen, int a_Probability) :
		m_Noise(a_Seed),
		m_Seed(a_Seed),
		m_Fluid(a_Fluid),
		m_HeiGen(a_HeiGen),
		m_Probability(a_Probability)
	{
	}
	
protected:
	cNoise              m_Noise;
	int                 m_Seed;
	BLOCKTYPE           m_Fluid;
	cTerrainHeightGen & m_HeiGen;
	int                 m_Probability;  ///< Chance, 0 .. 100, of a chunk having the lake
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
	
	/// Creates a lake image for the specified chunk into a_Lake
	void CreateLakeImage(int a_ChunkX, int a_ChunkZ, cBlockArea & a_Lake);
} ;






class cStructGenDirectOverhangs :
	public cFinishGen
{
public:
	cStructGenDirectOverhangs(int a_Seed);

protected:
	cNoise    m_Noise1;
	cNoise    m_Noise2;
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	bool HasWantedBiome(cChunkDesc & a_ChunkDesc) const;
} ;





class cStructGenDistortedMembraneOverhangs :
	public cFinishGen
{
public:
	cStructGenDistortedMembraneOverhangs(int a_Seed);

protected:
	cNoise m_NoiseX;
	cNoise m_NoiseY;
	cNoise m_NoiseZ;
	cNoise m_NoiseH;
	
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;




