
// Regions.h

// Declares the cRegions class representing individual regions to zap





#pragma once

#include <iostream>





struct cRegion
{
	int m_MinX, m_MaxX;
	int m_MinY, m_MaxY;
	int m_MinZ, m_MaxZ;

	bool m_ShouldZapBlocks;
	bool m_ShouldZapEntities;

	cRegion(void);
	cRegion(int a_MinX, int a_MaxX, int a_MinY, int a_MaxY, int a_MinZ, int a_MaxZ, bool a_ShouldZapBlocks, bool a_ShouldZapEntities);

	bool TouchesChunk(int a_ChunkX, int a_ChunkZ) const;
} ;

typedef std::vector<cRegion> cRegionVector;





class cRegions
{
public:
	/** Reads the list of regions from the specified stream */
	void Read(std::istream & a_Stream);

	/** Returns all regions in this container */
	const cRegionVector & GetAll(void) const { return m_Regions; }

protected:
	cRegionVector m_Regions;

	/** Adds a new region based on the contents of the split line. The split must already be the correct size */
	void AddRegion(const AStringVector & a_Split);

} ;
