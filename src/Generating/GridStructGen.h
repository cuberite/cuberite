
// GridStructGen.h

// Declares the cGridStructGen class representing a common base class for structure generators that place structures in a semi-random grid





#pragma once

#include "ComposableGenerator.h"
#include "../Noise/Noise.h"





/** Generates structures in a semi-random grid.
Defines a grid in the XZ space with predefined cell size in each direction. Each cell then receives exactly
one structure (provided by the descendant class). The structure is placed within the cell, but doesn't need
to be bounded by the cell, it can be well outside the cell; the generator uses the MaxStructureSize parameter
to determine how far away from the cell the structure can be at most. Each structure has an offset from the
grid's center point, the offset is generated randomly from a range given to this class as a parameter.

Each structure thus contains the coords of its grid center (m_GridX, m_GridZ) and the actual origin from
which it's built (m_OriginX, m_OriginZ).

This class provides a cache for the structures generated for successive chunks and manages that cache. It
also provides the cFinishGen override that uses the cache to actually generate the structure into chunk data.

After generating each chunk the cache is checked for size, each item in the cache has a cost associated with
it and the cache is trimmed (from its least-recently-used end) so that the sum of the cost in the cache is
less than m_MaxCacheSize

To use this class, declare a descendant class that implements the overridable methods, then create an
instance of that class. The descendant must provide the CreateStructure() function that is called to generate
a structure at the specific grid cell.

The descendant must use a specific cStructure descendant to provide the actual structure that gets generated.
The structure must provide the DrawIntoChunk() function that generates the structure into the chunk data, and
can override the GetCacheCost() function that returns the cost of that structure in the cache.
*/
class cGridStructGen :
	public cFinishGen
{
public:
	/** Represents a single structure that occupies the grid point. Knows how to draw itself into a chunk. */
	class cStructure
	{
	public:
		/** The grid point for which the structure is generated. */
		int m_GridX, m_GridZ;

		/** The origin (the coords for which the structure is generated) */
		int m_OriginX, m_OriginZ;


		/** Creates a structure that has its origin set at the specified coords. */
		cStructure (int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) :
			m_GridX(a_GridX),
			m_GridZ(a_GridZ),
			m_OriginX(a_OriginX),
			m_OriginZ(a_OriginZ)
		{
		}

		// Force a virtual destructor in descendants:
		virtual ~cStructure() {}

		/** Draws self into the specified chunk */
		virtual void DrawIntoChunk(cChunkDesc & a_ChunkDesc) = 0;

		/** Returns the cost of keeping this structure in the cache */
		virtual size_t GetCacheCost(void) const { return 1; }
	} ;
	typedef std::shared_ptr<cStructure> cStructurePtr;
	typedef std::list<cStructurePtr> cStructurePtrs;


	cGridStructGen(
		int a_Seed,
		int a_GridSizeX, int a_GridSizeZ,
		int a_MaxOffsetX, int a_MaxOffsetZ,
		int a_MaxStructureSizeX, int a_MaxStructureSizeZ,
		size_t a_MaxCacheSize
	);

	/** Creates a new instance that has the generation parameters set to defaults.
	This is used for instances that are later loaded from a file. */
	cGridStructGen(int a_Seed);

	/** Sets the generator params based on the dictionary passed in.
	Note that this must not be called anymore after generating a chunk. */
	void SetGeneratorParams(const AStringMap & a_GeneratorParams);

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

protected:
	/** Base seed of the world for which the generator generates chunk. */
	int m_BaseSeed;

	/** Seed for generating grid offsets and also available for descendants.
	Calculated from m_BaseSeed by adding the SeedOffset parameter loaded from the cubeset file (if applicable); otherwise the same as m_BaseSeed. */
	int m_Seed;

	/** The noise used for generating grid offsets. */
	cNoise m_Noise;

	/** The size of each grid's cell in the X axis */
	int m_GridSizeX;

	/** The size of each grid's cell in the Z axis */
	int m_GridSizeZ;

	/** The maximum offset of the structure's origin from the grid midpoint, in X coord. */
	int m_MaxOffsetX;

	/** The maximum offset of the structure's origin from the grid midpoint, in Z coord. */
	int m_MaxOffsetZ;

	/** Maximum theoretical size of the structure in the X axis.
	This limits the structures considered for a single chunk, so the lesser the number, the better performance.
	Structures large than this may get cropped. */
	int m_MaxStructureSizeX;

	/** Maximum theoretical size of the structure in the Z axis.
	This limits the structures considered for a single chunk, so the lesser the number, the better performance.
	Structures large than this may get cropped. */
	int m_MaxStructureSizeZ;

	/** Maximum allowed sum of costs for items in the cache. Items that are over this cost are removed from the
	cache, oldest-first */
	size_t m_MaxCacheSize;

	/** Cache for the most recently generated structures, ordered by the recentness. */
	cStructurePtrs m_Cache;


	/** Clears everything from the cache */
	void ClearCache(void);

	/** Returns all structures that may intersect the given chunk.
	The structures are considered as intersecting iff their bounding box (defined by m_MaxStructureSize)
	around their gridpoint intersects the chunk. */
	void GetStructuresForChunk(int a_ChunkX, int a_ChunkZ, cStructurePtrs & a_Structures);

	// Functions for the descendants to override:
	/** Create a new structure at the specified gridpoint */
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) = 0;
} ;





