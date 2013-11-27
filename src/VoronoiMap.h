
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
	
	/// Returns the value in the cell into which the specified point lies, and the distances to the 2 nearest Voronoi seeds
	int GetValueAt(int a_X, int a_Y, int & a_MinDistance1, int & a_MinDistance2);
	
protected:
	/// The noise used for generating Voronoi seeds
	cNoise m_Noise;
	
	/// Size of the Voronoi cells (avg X/Y distance between the seeds)
	int m_CellSize;
} ;




