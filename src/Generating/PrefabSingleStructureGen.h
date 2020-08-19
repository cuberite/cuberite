
// PrefabSingleStructureGen.h


/*
Classes to support the generation of single piece prefab structures
*/

#pragma once

#include "Globals.h"
#include "GridStructGen.h"
#include "../BlockInfo.h"


/** Enum of all implemented Structures */
enum class PrefabSingleStructure
{
	DesertTemple,
	JungleTemple,
	WitchHut,
	DesertWell
};

/** A class to represent the Prefab of the generated structure
	This is used every time a Structure is placed to prevent
	that the structure is loaded from disk every time */
class cPrefabSinglePiece
{
public:
	cPrefabSinglePiece();

	/** Reads the Prefab from file and stores it as a INSERT DATATYPE */
	void ReadFromFile(AString & a_Filename);

	// Todo: store the Prefab in memory
};




/** A class representing an individual structure on the map */
class cPrefabSingleStructure:
	public cGridStructGen::cStructure
{
	using Super = cGridStructGen::cStructure;

	cPrefabSingleStructure
	(
		int a_Seed,
		int a_GridX, int a_GridZ,
		int a_OriginX, int a_OriginZ,
		cTerrainHeightGenPtr a_HeightGen
	);

	/** cGridStructGen::cStructure override */
	virtual void DrawIntoChunk(cChunkDesc &a_ChunkDesc) override;

protected:
	/** Seed for the random functions */
	int m_Seed;

	/** The noise used as a pseudo-random generator */
	cNoise m_Noise;

	/** The underlying height generator, used for placing the structures on top of the terrain. */
	cTerrainHeightGenPtr m_HeightGen;
};




/** The Prefab Single Structure Generator:
	This uses the cGridStructGen to generate the structures on the map
	This is dependent on the PrefabSingleStructure to make the behaviour more universal */
class cPrefabSingleStructureGen :
	public cGridStructGen
{
using Super = cGridStructGen;

public:

cPrefabSingleStructureGen(
	int a_Seed,
	int a_GridSize,
	int a_MaxOffset,
	int a_MaxDepth,
	cBiomeGenPtr a_BiomeGen,
	cTerrainHeightGenPtr a_HeightGen,
	int a_SeaLevel,
	enum PrefabSingleStructure a_Type
);

cGridStructGen::cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;

protected:
/** The noise used for generating random numbers */
	cNoise m_RandNoise;

	/** Maximum depth of the generator tree */
	int m_MaxDepth;

	/** The underlying biome generator that defines whether the village is created or not */
	cBiomeGenPtr m_BiomeGen;

	/** The underlying height generator, used to position the prefabs crossing chunk borders */
	cTerrainHeightGenPtr m_HeightGen;

	/** Storing the type */
	enum PrefabSingleStructure m_Type;

	/** A container for the Prefab loaded from file */
	cPrefabSinglePiece m_Piece;
};
