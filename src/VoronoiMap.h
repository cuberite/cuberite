
// VoronoiMap.h

// Declares the cVoronoiMap class that implements a Voronoi algorithm over a noise to produce a map





#pragma once

#include "Noise/Noise.h"





class cVoronoiMap
{
public:
	cVoronoiMap(int a_Seed, int a_CellSize = 128, int a_JitterSize = 128);

	/** Sets both the cell size and jitter size used for generating the Voronoi seeds. */
	void SetCellSize(int a_CellSize);

	/** Sets the jitter size. Clamps it to current cell size. */
	void SetJitterSize(int a_JitterSize);

	/** Sets the offset that is added to each odd row of cells.
	This offset makes the voronoi cells align to a non-grid.
	Clamps the value to [-m_CellSize, +m_CellSize]. */
	void SetOddRowOffset(int a_OddRowOffset);

	/** Returns the value in the cell into which the specified point lies. */
	int GetValueAt(int a_X, int a_Y);

	/** Returns the value in the cell into which the specified point lies,
	and the distance to the nearest Voronoi seed. */
	int GetValueAt(int a_X, int a_Y, int & a_MinDistance);

	/** Returns the value in the cell into which the specified point lies,
	and the distances to the 2 nearest Voronoi seeds. Uses a cache. */
	int GetValueAt(
		int a_X, int a_Y,                            // Coords to query
		int & a_NearestSeedX, int & a_NearestSeedY,  // Coords of the closest cell's seed
		int & a_MinDist2                             // Distance to the second closest cell's seed
	);

	/** Finds the nearest and second nearest seeds, returns their coords. */
	void FindNearestSeeds(
		int a_X, int a_Y,
		int & a_NearestSeedX, int & a_NearestSeedY,
		int & a_SecondNearestSeedX, int & a_SecondNearestSeedY
	);

protected:
	/** The noise used for generating Voronoi seeds */
	cNoise m_Noise1;
	cNoise m_Noise2;
	cNoise m_Noise3;

	/** Size of the Voronoi cells (avg X / Y distance between the seeds). Expected to be at least 2. */
	int m_CellSize;

	/** The amount that the cell seeds may be offset from the grid.
	Expected to be at least 1 and less than m_CellSize. */
	int m_JitterSize;

	/** The constant amount that the cell seeds of every odd row will be offset from the grid.
	This allows us to have non-rectangular grids.
	Expected to be between -m_CellSize and +m_CellSize. */
	int m_OddRowOffset;

	/** The X coordinate of the currently cached cell neighborhood */
	int m_CurrentCellX;

	/** The Z coordinate of the currently cached cell neighborhood */
	int m_CurrentCellZ;

	/** The seeds of cells around m_CurrentCellX, m_CurrentCellZ, X-coords */
	int m_SeedX[5][5];

	/** The seeds of cells around m_CurrentCellX, m_CurrentCellZ, X-coords */
	int m_SeedZ[5][5];


	/** Updates the cached cell seeds to match the specified cell. Noop if cell pos already matches.
	Updates m_SeedX and m_SeedZ. */
	void UpdateCell(int a_CellX, int a_CellZ);
} ;




