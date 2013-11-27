
// VoronoiMap.cpp

// Implements the cVoronoiMap class that implements a Voronoi algorithm over a noise to produce a map

#include "Globals.h"
#include "VoronoiMap.h"





cVoronoiMap::cVoronoiMap(int a_Seed, int a_CellSize) :
	m_Noise(a_Seed),
	m_CellSize(a_CellSize)
{
}





void cVoronoiMap::SetCellSize(int a_CellSize)
{
	m_CellSize = a_CellSize;
}





int cVoronoiMap::GetValueAt(int a_X, int a_Y)
{
	int MinDist1, MinDist2;
	return GetValueAt(a_X, a_Y, MinDist1, MinDist2);
}





int cVoronoiMap::GetValueAt(int a_X, int a_Y, int & a_MinDist)
{
	int MinDist2;
	return GetValueAt(a_X, a_Y, a_MinDist, MinDist2);
}





int cVoronoiMap::GetValueAt(int a_X, int a_Y, int & a_MinDist1, int & a_MinDist2)
{
	// Note that due to historical reasons, the algorithm uses XZ coords, while the input uses XY coords.
	// This is because the algorithm was first implemented directly in the biome generators which use MC coords.
	
	int CellX = a_X / m_CellSize;
	int CellZ = a_Y / m_CellSize;
	
	// Get 5x5 neighboring cell seeds, compare distance to each. Return the value in the minumim-distance cell
	int MinDist = m_CellSize * m_CellSize * 16;  // There has to be a cell closer than this
	int MinDist2 = MinDist;
	int res = 0;  // Will be overriden
	for (int x = CellX - 2; x <= CellX + 2; x++)
	{
		int BaseX = x * m_CellSize;
		for (int z = CellZ - 2; z < CellZ + 2; z++)
		{
			int OffsetX = (m_Noise.IntNoise3DInt(x, 16 * x + 32 * z, z) / 8) % m_CellSize;
			int OffsetZ = (m_Noise.IntNoise3DInt(x, 32 * x - 16 * z, z) / 8) % m_CellSize;
			int SeedX = BaseX + OffsetX;
			int SeedZ = z * m_CellSize + OffsetZ;
			
			int Dist = (SeedX - a_X) * (SeedX - a_X) + (SeedZ - a_Y) * (SeedZ - a_Y);
			if (Dist < MinDist)
			{
				MinDist2 = MinDist;
				MinDist = Dist;
				res = m_Noise.IntNoise3DInt(x, x - z + 1000, z);
			}
			else if (Dist < MinDist2)
			{
				MinDist2 = Dist;
			}
		}  // for z
	}  // for x

	a_MinDist1 = MinDist;
	a_MinDist2 = MinDist2;
	return res;
}




