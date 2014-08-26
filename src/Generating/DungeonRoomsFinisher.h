
// DungeonRoomsFinisher.h

// Declares the cDungeonRoomsFinisher class representing the finisher that generates dungeon rooms





#pragma once

#include "GridStructGen.h"





class cDungeonRoomsFinisher :
	public cGridStructGen
{
	typedef cGridStructGen super;

public:
	/** Creates a new dungeon room finisher.
	a_HeightGen is the underlying height generator, so that the rooms can always be placed under the terrain.
	a_MaxSize and a_MinSize are the maximum and minimum sizes of the room's internal (air) area, in blocks across. */
	cDungeonRoomsFinisher(cTerrainHeightGen & a_HeightGen, int a_Seed, int a_GridSize, int a_MaxSize, int a_MinSize);

protected:

	/** The height gen that is used for limiting the rooms' Y coords */
	cTerrainHeightGen & m_HeightGen;

	/** Maximum half-size (from center to wall) of the dungeon room's inner (air) area. Default is 3 (vanilla). */
	int m_MaxHalfSize;

	/** Minimum half-size (from center to wall) of the dungeon room's inner (air) area. Default is 2 (vanilla). */
	int m_MinHalfSize;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




