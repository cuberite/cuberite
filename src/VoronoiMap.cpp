
// VoronoiMap.cpp

// Implements the cVoronoiMap class that implements a Voronoi algorithm over a noise to produce a map

#include "Globals.h"
#include "VoronoiMap.h"





cVoronoiMap::cVoronoiMap(int a_Seed, int a_CellSize) :
	m_Noise1(a_Seed + 1),
	m_Noise2(a_Seed + 2),
	m_Noise3(a_Seed + 3),
	m_CellSize(a_CellSize),
	m_CurrentCellX(9999999),  // Cell coords that are definitely out of the range for normal generator, so that the first query will overwrite them
	m_CurrentCellZ(9999999)
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
	
	UpdateCell(CellX, CellZ);
	
	// Get 5x5 neighboring cell seeds, compare distance to each. Return the value in the minumim-distance cell
	int MinDist = m_CellSize * m_CellSize * 16;  // There has to be a cell closer than this
	int MinDist2 = MinDist;
	int res = 0;  // Will be overriden
	for (int x = 0; x < 5; x++)
	{
		for (int z = 0; z < 5; z++)
		{
			int SeedX = m_SeedX[x][z];
			int SeedZ = m_SeedZ[x][z];
			
			int Dist = (SeedX - a_X) * (SeedX - a_X) + (SeedZ - a_Y) * (SeedZ - a_Y);
			if (Dist < MinDist)
			{
				MinDist2 = MinDist;
				MinDist = Dist;
				res = m_Noise3.IntNoise2DInt(x + CellX - 2, z + CellZ - 2);
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





void cVoronoiMap::UpdateCell(int a_CellX, int a_CellZ)
{
	// If the specified cell is currently cached, bail out:
	if ((a_CellX == m_CurrentCellX) && (a_CellZ == m_CurrentCellZ))
	{
		return;
	}
	
	// Update the cell cache for the new cell position:
	int NoiseBaseX = a_CellX - 2;
	int NoiseBaseZ = a_CellZ - 2;
	for (int x = 0; x < 5; x++)
	{
		int BaseX = (NoiseBaseX + x) * m_CellSize;
		for (int z = 0; z < 5; z++)
		{
			int OffsetX = (m_Noise1.IntNoise2DInt(NoiseBaseX + x, NoiseBaseZ + z) / 8) % m_CellSize;
			int OffsetZ = (m_Noise2.IntNoise2DInt(NoiseBaseX + x, NoiseBaseZ + z) / 8) % m_CellSize;
			m_SeedX[x][z] = BaseX + OffsetX;
			m_SeedZ[x][z] = (NoiseBaseZ + z) * m_CellSize + OffsetZ;
		}  // for z
	}  // for x
	m_CurrentCellX = a_CellX;
	m_CurrentCellZ = a_CellZ;
}




