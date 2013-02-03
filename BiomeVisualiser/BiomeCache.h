
// BiomeCache.h

// Declares the cBiomeCache class representing a biome source that caches data from the underlying biome source

/*
This cache works a bit differently than regular caches. 
It first receives the hint of area that it will need to provide.
The Cache uses several threads to request biomes from the underlying source to fill that area.
While the area is being filled, requests for biomes may already come, such requests are answered with baLater if no data yet.
*/

#pragma once





#include "BiomeSource.h"
#include "../source/OSSupport/IsThread.h"





class cBiomeCache :
	public cBiomeSource
{
public:
	cBiomeCache(void);
	~cBiomeCache();
	
	// cBiomeSource overrides:
	virtual eAvailability GetBiome(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_Biomes) override;
	virtual void HintViewArea(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ) override;
	
	void SetSource(cBiomeSource * a_Source);  // Takes ownership of the source ptr
	
protected:
	class cItem
	{
	public:
		cItem(int a_ChunkX, int a_ChunkZ);
		
		int m_ChunkX;
		int m_ChunkZ;
		bool m_IsValid;
		cChunkDef::BiomeMap m_Biomes;
	} ;
	
	typedef cItem * pItem;
	typedef std::list<pItem> cItems;
	
	class cThread :
		public cIsThread
	{
		typedef cIsThread super;
		
	public:
		cThread(cBiomeCache & a_Parent);
		
		// cIsThread overrides:
		virtual void Execute(void) override;
		
	protected:
		cBiomeCache & m_Parent;
	} ;
	
	typedef std::list<cThread *> cThreads;
	
	cBiomeSource * m_Source;
	
	cCriticalSection m_CS;
	int     m_BaseX;      ///< MinChunkX for the m_Available rectangle
	int     m_BaseZ;      ///< MinChunkZ for the m_Available rectangle
	int     m_Width;      ///< Width of the m_Available rectangle
	int     m_Height;     ///< Height of the m_Available rectangle
	pItem * m_Available;  ///< Items that have already been processed (baNow or baNever), [x + m_Width * z]
	cItems  m_Queue;      ///< Items that are queued for processing (baLater)
	cItems  m_Pool;       ///< Items that are not needed anymore, can be reused for other coords
	
	cEvent m_evtQueued;  // Triggerred when an item is added to m_Queue
	
	cThreads m_Threads;  // Threads that update the cache.
	bool m_IsTerminatingThreads;  // Set to true to indicate to all threads that they should exit
	
	/// Removes from a_Items all items that are outside of the given coords, moves those into m_Pool
	void FilterOutItems(cItems & a_Items, int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ);
	
	/// Processes one item from m_Queue into m_Available. Blocks if m_Queue is empty; respects m_IsTerminatingThreads
	void thrProcessQueueItem(void);
} ;




