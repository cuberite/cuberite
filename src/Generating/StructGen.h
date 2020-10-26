
// StructGen.h

/* Interfaces to the various structure generators:
	- cStructGenTrees
	- cStructGenMarbleCaves
	- cStructGenOres
*/





#pragma once

#include "ComposableGenerator.h"
#include "../Noise/Noise.h"



class cStructGenTrees :
	public cFinishGen
{
public:
	cStructGenTrees(int a_Seed, cBiomeGenPtr a_BiomeGen, cTerrainShapeGenPtr a_ShapeGen, cTerrainCompositionGenPtr a_CompositionGen) :
		m_Seed(a_Seed),
		m_Noise(a_Seed),
		m_BiomeGen(std::move(a_BiomeGen)),
		m_ShapeGen(std::move(a_ShapeGen)),
		m_CompositionGen(std::move(a_CompositionGen))
	{}

protected:

	int m_Seed;
	cNoise m_Noise;
	cBiomeGenPtr              m_BiomeGen;
	cTerrainShapeGenPtr       m_ShapeGen;
	cTerrainCompositionGenPtr m_CompositionGen;

	/** Generates and applies an image of a single tree.
	Parts of the tree inside the chunk are applied to a_ChunkDesc.
	Parts of the tree outside the chunk are stored in a_OutsideXYZ
	*/
	void GenerateSingleTree(
		int a_ChunkX, int a_ChunkZ, int a_Seq,
		Vector3i a_Pos,
		cChunkDesc & a_ChunkDesc,
		sSetBlockVector & a_OutsideLogs,
		sSetBlockVector & a_OutsideOther
	) ;

	/** Applies an image into chunk blockdata; all blocks outside the chunk will be appended to a_Overflow. */
	void ApplyTreeImage(
		int a_ChunkX, int a_ChunkZ,
		cChunkDesc & a_ChunkDesc,
		const sSetBlockVector & a_Image,
		sSetBlockVector & a_Overflow
	);

	/** Get the the number of trees to generate in a_Chunk
	If the value is between 0 and 1, it should be interpreted as the probability that a tree should be generated.
	*/
	double GetNumTrees(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BiomeMap & a_Biomes
	);

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cStructGenLakes :
	public cFinishGen
{
public:
	cStructGenLakes(int a_Seed, BLOCKTYPE a_Fluid, cTerrainShapeGenPtr a_ShapeGen, int a_Probability) :
		m_Noise(a_Seed),
		m_Seed(a_Seed),
		m_Fluid(a_Fluid),
		m_ShapeGen(std::move(a_ShapeGen)),
		m_Probability(a_Probability)
	{
	}

protected:
	cNoise              m_Noise;
	int                 m_Seed;
	BLOCKTYPE           m_Fluid;
	cTerrainShapeGenPtr m_ShapeGen;

	/** Chance, [0 .. 100], of a chunk having the lake. */
	int m_Probability;


	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	/** Creates a lake image for the specified chunk into a_Lake. */
	void CreateLakeImage(int a_ChunkX, int a_ChunkZ, int a_MaxLakeHeight, cBlockArea & a_Lake);
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




