
// BiomeSource.h

// Declares the cBiomeSource abstract class used as an interface for getting biomes from any source

#pragma once





#include "ChunkDef.h"





class cBiomeSource abstract
{
public:
	enum eAvailability
	{
		baNow,    // Data returned now
		baLater,  // Data not returned, but will be available later, try again after a while
		baNever,  // Data not returned, will not be available at all
	} ;
	
	/// Fills a_Biomes with the biomes for the chunk specified
	virtual eAvailability GetBiome(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_Biomes) = 0;
	
	/// Used to inform the source about the view area that will be queried in the near future.
	virtual void HintViewArea(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ) = 0;
} ;




