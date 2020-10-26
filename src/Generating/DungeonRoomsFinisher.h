
// DungeonRoomsFinisher.h

// Declares the cDungeonRoomsFinisher class representing the finisher that generates dungeon rooms





#pragma once

#include "GridStructGen.h"
#include "../ProbabDistrib.h"





class cDungeonRoomsFinisher:
	public cGridStructGen
{
	using Super = cGridStructGen;

public:

	/** Creates a new dungeon room finisher.
	a_ShapeGen is the underlying terrain shape generator, so that the rooms can always be placed under the terrain.
	a_MaxSize and a_MinSize are the maximum and minimum sizes of the room's internal (air) area, in blocks across.
	a_HeightDistrib is the string defining the height distribution for the rooms (cProbabDistrib format). */
	cDungeonRoomsFinisher(cTerrainShapeGenPtr a_ShapeGen, int a_Seed, int a_GridSize, int a_MaxSize, int a_MinSize, const AString & a_HeightDistrib);

protected:

	/** The shape gen that is used for limiting the rooms' Y coords */
	cTerrainShapeGenPtr m_ShapeGen;

	/** Maximum half-size (from center to wall) of the dungeon room's inner (air) area. Default is 3 (vanilla). */
	int m_MaxHalfSize;

	/** Minimum half-size (from center to wall) of the dungeon room's inner (air) area. Default is 2 (vanilla). */
	int m_MinHalfSize;

	/** The height probability distribution to make the spawners more common in layers 10 - 40, less common outside this range. */
	cProbabDistrib m_HeightProbability;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




