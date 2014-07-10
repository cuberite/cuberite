
// VoronoiMap.h

// Declares the cVoronoiMap class that implements a Voronoi algorithm over a noise to produce a map





#pragma once

#include "Noise.h"





class cVoronoiMap
{
public:
	cVoronoiMap(int a_Seed, int a_CellSize = 128);
	
	/// Sets the cell size used for generating the Voronoi seeds
	void SetCellSize(int a_CellSize);
	
	/// Returns the value in the cell into which the specified point lies
	int GetValueAt(int a_X, int a_Y);
	
	/// Returns the value in the cell into which the specified point lies, and the distance to the nearest Voronoi seed
	int GetValueAt(int a_X, int a_Y, int & a_MinDistance);
	
	/// Returns the value in the cell into which the specified point lies, and the distances to the 2 nearest Voronoi seeds. Uses a cache
	int GetValueAt(int a_X, int a_Y, int & a_MinDistance1, int & a_MinDistance2);

protected:
	/// The noise used for generating Voronoi seeds
	cNoise m_Noise1;
	cNoise m_Noise2;
	cNoise m_Noise3;
	
	/// Size of the Voronoi cells (avg X/Y distance between the seeds)
	int m_CellSize;
	
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




