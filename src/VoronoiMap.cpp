
// VoronoiMap.cpp

// Implements the cVoronoiMap class that implements a Voronoi algorithm over a noise to produce a map

#include "Globals.h"
#include "VoronoiMap.h"





cVoronoiMap::cVoronoiMap(int a_Seed, int a_CellSize, int a_JitterSize) :
	m_Noise1(a_Seed + 1),
	m_Noise2(a_Seed + 2),
	m_Noise3(a_Seed + 3),
	m_CellSize(std::max(a_CellSize, 2)),
	m_JitterSize(Clamp(a_JitterSize, 1, a_CellSize)),
	m_OddRowOffset(0),
	m_CurrentCellX(9999999),  // Cell coords that are definitely out of the range for normal generator, so that the first query will overwrite them
	m_CurrentCellZ(9999999)
{
}





void cVoronoiMap::SetCellSize(int a_CellSize)
{
	a_CellSize = std::max(a_CellSize, 2);  // Cell size must be at least 2
	m_CellSize = a_CellSize;

	// For compatibility with previous version, which didn't have the jitter, we set jitter here as well.
	m_JitterSize = a_CellSize;
}





void cVoronoiMap::SetJitterSize(int a_JitterSize)
{
	m_JitterSize = Clamp(a_JitterSize, 1, m_CellSize);
}





void cVoronoiMap::SetOddRowOffset(int a_OddRowOffset)
{
	m_OddRowOffset = Clamp(a_OddRowOffset, -m_CellSize, m_CellSize);
}





int cVoronoiMap::GetValueAt(int a_X, int a_Y)
{
	int SeedX, SeedY, MinDist2;
	return GetValueAt(a_X, a_Y, SeedX, SeedY, MinDist2);
}





int cVoronoiMap::GetValueAt(int a_X, int a_Y, int & a_MinDist)
{
	int SeedX, SeedY, MinDist2;
	int res = GetValueAt(a_X, a_Y, SeedX, SeedY, MinDist2);
	a_MinDist = (a_X - SeedX) * (a_X - SeedX) + (a_Y - SeedY) * (a_Y - SeedY);
	return res;
}





int cVoronoiMap::GetValueAt(
	int a_X, int a_Y,  // Coords to query
	int & a_NearestSeedX, int & a_NearestSeedY,  // Coords of the closest cell
	int & a_MinDist2  // Distance to the second closest cell
)
{
	int CellX = a_X / m_CellSize;
	int CellY = a_Y / m_CellSize;

	UpdateCell(CellX, CellY);

	// Get 5x5 neighboring cell seeds, compare distance to each. Return the value in the minumim-distance cell
	int NearestSeedX = 0, NearestSeedY = 0;
	int MinDist = m_CellSize * m_CellSize * 16;  // There has to be a cell closer than this
	int MinDist2 = MinDist;
	int res = 0;  // Will be overriden
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			int SeedX = m_SeedX[x][y];
			int SeedY = m_SeedZ[x][y];

			int Dist = (SeedX - a_X) * (SeedX - a_X) + (SeedY - a_Y) * (SeedY - a_Y);
			if (Dist < MinDist)
			{
				NearestSeedX = SeedX;
				NearestSeedY = SeedY;
				MinDist2 = MinDist;
				MinDist = Dist;
				res = m_Noise3.IntNoise2DInt(x + CellX - 2, y + CellY - 2);
			}
			else if (Dist < MinDist2)
			{
				MinDist2 = Dist;
			}
		}  // for z
	}  // for x

	a_NearestSeedX = NearestSeedX;
	a_NearestSeedY = NearestSeedY;
	a_MinDist2 = MinDist2;
	return res;
}





void cVoronoiMap::FindNearestSeeds(
	int a_X, int a_Y,
	int & a_NearestSeedX, int & a_NearestSeedY,
	int & a_SecondNearestSeedX, int & a_SecondNearestSeedY
)
{
	int CellX = a_X / m_CellSize;
	int CellY = a_Y / m_CellSize;

	UpdateCell(CellX, CellY);

	// Get 5x5 neighboring cell seeds, compare distance to each. Return the value in the minumim-distance cell
	int NearestSeedX = 0, NearestSeedY = 0;
	int SecondNearestSeedX = 0, SecondNearestSeedY = 0;
	int MinDist = m_CellSize * m_CellSize * 16;  // There has to be a cell closer than this
	int MinDist2 = MinDist;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			int SeedX = m_SeedX[x][y];
			int SeedY = m_SeedZ[x][y];

			int Dist = (SeedX - a_X) * (SeedX - a_X) + (SeedY - a_Y) * (SeedY - a_Y);
			if (Dist < MinDist)
			{
				SecondNearestSeedX = NearestSeedX;
				SecondNearestSeedY = NearestSeedY;
				MinDist2 = MinDist;
				NearestSeedX = SeedX;
				NearestSeedY = SeedY;
				MinDist = Dist;
			}
			else if (Dist < MinDist2)
			{
				SecondNearestSeedX = SeedX;
				SecondNearestSeedY = SeedY;
				MinDist2 = Dist;
			}
		}  // for z
	}  // for x

	a_NearestSeedX = NearestSeedX;
	a_NearestSeedY = NearestSeedY;
	a_SecondNearestSeedX = SecondNearestSeedX;
	a_SecondNearestSeedY = SecondNearestSeedY;
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
		int OddRowOffset = ((NoiseBaseX + x) & 0x01) * m_OddRowOffset;
		for (int z = 0; z < 5; z++)
		{
			int OffsetX = (m_Noise1.IntNoise2DInt(NoiseBaseX + x, NoiseBaseZ + z) / 8) % m_JitterSize;
			int OffsetZ = (m_Noise2.IntNoise2DInt(NoiseBaseX + x, NoiseBaseZ + z) / 8) % m_JitterSize;
			m_SeedX[x][z] = BaseX + OffsetX;
			m_SeedZ[x][z] = (NoiseBaseZ + z) * m_CellSize + OddRowOffset + OffsetZ;
		}  // for z
	}  // for x
	m_CurrentCellX = a_CellX;
	m_CurrentCellZ = a_CellZ;
}




