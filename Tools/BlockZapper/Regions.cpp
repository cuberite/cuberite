
// Regions.cpp

// Implements the cRegions class representing the list of regions to zap

#include "Globals.h"

#include "Regions.h"





////////////////////////////////////////////////////////////////////////////////
// cRegion:

cRegion::cRegion(void) :
	m_MinX(0),
	m_MaxX(0),
	m_MinY(0),
	m_MaxY(0),
	m_MinZ(0),
	m_MaxZ(0),
	m_ShouldZapBlocks(false),
	m_ShouldZapEntities(false)
{
}





cRegion::cRegion(int a_MinX, int a_MaxX, int a_MinY, int a_MaxY, int a_MinZ, int a_MaxZ, bool a_ShouldZapBlocks, bool a_ShouldZapEntities) :
	m_MinX(a_MinX),
	m_MaxX(a_MaxX),
	m_MinY(std::max(0, std::min(255, a_MinY))),
	m_MaxY(std::max(0, std::min(255, a_MaxY))),
	m_MinZ(a_MinZ),
	m_MaxZ(a_MaxZ),
	m_ShouldZapBlocks(a_ShouldZapBlocks),
	m_ShouldZapEntities(a_ShouldZapEntities)
{
}





bool cRegion::TouchesChunk(int a_ChunkX, int a_ChunkZ) const
{
	int ChunkBeginX = a_ChunkX * 16;
	int ChunkEndX = a_ChunkX * 16 + 15;
	int ChunkBeginZ = a_ChunkZ * 16;
	int ChunkEndZ = a_ChunkZ * 16 + 15;
	if (
		(m_MinX > ChunkEndX) || (m_MaxX < ChunkBeginX) ||
		(m_MinZ > ChunkEndZ) || (m_MaxZ < ChunkBeginZ)
	)
	{
		return false;
	}
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cRegions:

void cRegions::Read(std::istream & a_Stream)
{
	while (!a_Stream.eof())
	{
		AString Line;
		std::getline(a_Stream, Line);

		// Process the line
		AStringVector Split = StringSplit(Line, " \t");
		AStringVector NonEmpty;
		for (AStringVector::const_iterator itr = Split.begin(), end = Split.end(); itr != end; ++itr)
		{
			if (!itr->empty())
			{
				NonEmpty.push_back(*itr);
			}
		}  // for itr - Split[]
		switch (NonEmpty.size())
		{
			case 6:
			case 7:
			{
				AddRegion(NonEmpty);
				break;
			}
			default:
			{
				fprintf(stderr, "Cannot parse line \"%s\", ignoring", Line.c_str());
				break;
			}
		}
	}
}





void cRegions::AddRegion(const AStringVector & a_Split)
{
	ASSERT((a_Split.size() == 6) || (a_Split.size() == 7));

	int Coords[6];
	for (int i = 0; i < 6; i++)
	{
		Coords[i] = atoi(a_Split[i].c_str());
		if ((Coords[i] == 0) && (a_Split[i] != "0"))
		{
			fprintf(stderr, "Bad coord: \"%s\". Ignoring line.", a_Split[i].c_str());
			return;
		}
	}  // for i - a_Split[]

	bool ShouldZapBlocks = true;
	bool ShouldZapEntities = false;

	if (a_Split.size() == 7)
	{
		AString Upper = a_Split[6];
		StrToUpper(Upper);
		if (Upper == "E")
		{
			ShouldZapEntities = true;
			ShouldZapBlocks = false;
		}
		else if (Upper == "BE")
		{
			ShouldZapEntities = true;
		}
		else if (Upper == "B")
		{
			// Nothing needed
		}
		else
		{
			fprintf(stderr, "Bad zap specifier: \"%s\". Ignoring line.", a_Split[6].c_str());
			return;
		}
	}

	// Swap coords, if needed:
	for (int i = 0; i < 3; i++)
	{
		if (Coords[2 * i] > Coords[2 * i + 1])
		{
			std::swap(Coords[2 * i], Coords[2 * i + 1]);
		}
	}

	// Store the region
	m_Regions.push_back(cRegion(Coords[0], Coords[1], Coords[2], Coords[3], Coords[4], Coords[5], ShouldZapBlocks, ShouldZapEntities));
}
