
// Callback.h

// Interfaces to the cCallback base class used as the base class for all statistical callbacks





#pragma once





/** The base class for all chunk-processor callbacks, declares the interface.
The processor calls each virtual function in the order they are declared here with the specified args.
If the function returns true, the processor moves on to next chunk and starts calling the callbacks again from start with
the new chunk data.
So if a statistics collector doesn't need data decompression at all, it can stop the processor from doing so early-enough
and still get meaningful data.
A callback is guaranteed to run in a single thread and always the same thread.
A callback is guaranteed to run on all chunks in a region and one region is guaranteed to be handled by only callback.
*/
class cCallback abstract
{
public:
	virtual ~cCallback() {}  // Force a virtual destructor in each descendant
	
	/// Called to inform the stats module of the chunk coords for newly processing chunk
	virtual bool OnNewChunk(int a_ChunkX, int a_ChunkZ) = 0;
	
	/// Called to inform about the chunk's data offset in the file (chunk mini-header), the number of sectors it uses and the timestamp field value
	virtual bool OnHeader(int a_FileOffset, unsigned char a_NumSectors, int a_Timestamp) { return true; }
	
	/// Called to inform of the compressed chunk data size and position in the file (offset from file start to the actual data)
	virtual bool OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod) { return true; }
	
	/// Just in case you wanted to process the NBT yourself ;)
	virtual bool OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize) { return true; }
	
	/// The chunk's NBT should specify chunk coords, these are sent here:
	virtual bool OnRealCoords(int a_ChunkX, int a_ChunkZ) { return true; }
	
	/// The chunk contains a LastUpdate value specifying the last tick in which it was saved.
	virtual bool OnLastUpdate(Int64 a_LastUpdate) { return true; }
	
	virtual bool OnTerrainPopulated(bool a_Populated) { return true; }
	
	virtual bool OnBiomes(const unsigned char * a_BiomeData) { return true; }
	
	virtual bool OnHeightMap(const int * a_HeightMap) { return true; }
	
	virtual bool OnSection(
		unsigned char a_Y,
		const BLOCKTYPE * a_BlockTypes,
		const NIBBLETYPE * a_BlockAdditional,
		const NIBBLETYPE * a_BlockMeta,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_BlockSkyLight
	) { return true; }
	
	// TODO: entities, tile-entities, tile-ticks
} ;

typedef std::vector<cCallback *> cCallbacks;





/** The base class for a factory that creates callback objects for separate threads.
The processor creates a callback for each thread on which it runs using this factory.
The factory is guaranteed to be called from a single thread.
The factory keeps track of all the callbacks that it has created and deletes them when destructed
*/
class cCallbackFactory
{
public:
	virtual ~cCallbackFactory()
	{
		for (cCallbacks::iterator itr = m_Callbacks.begin(), end = m_Callbacks.end(); itr != end; ++itr)
		{
			delete *itr;
		}
	}
	
	/// Descendants override this method to return the correct callback type
	virtual cCallback * CreateNewCallback(void) = 0;
	
	/// cProcessor uses this method to request a new callback
	cCallback * GetNewCallback(void)
	{
		cCallback * Callback = CreateNewCallback();
		if (Callback != NULL)
		{
			m_Callbacks.push_back(Callback);
		}
		return Callback;
	}
	
protected:
	cCallbacks m_Callbacks;
} ;




