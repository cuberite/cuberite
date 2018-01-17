
// Callback.h

// Interfaces to the cCallback base class used as the base class for all statistical callbacks





#pragma once





// fwd:
class cParsedNBT;





/** The base class for all chunk-processor callbacks, declares the interface.
The processor calls each virtual function in the order they are declared here with the specified args.
If the function returns true, the processor doesn't process the data item, moves on to the next chunk
and starts calling the callbacks again from start with the new chunk data.
So if a statistics collector doesn't need data decompression at all, it can stop the processor from doing so early-enough
and still get meaningful data.
A callback is guaranteed to run in a single thread and always the same thread for the same chunk.
A callback is guaranteed to run on all chunks in a region and one region is guaranteed to be handled by only callback.
*/
class cCallback abstract
{
public:
	enum
	{
		CALLBACK_CONTINUE = false,
		CALLBACK_ABORT    = true,
	} ;

	virtual ~cCallback() {}  // Force a virtual destructor in each descendant

	/** Called when a new region file is about to be opened; by default allow the region */
	virtual bool OnNewRegion(int a_RegionX, int a_RegionZ) { return CALLBACK_CONTINUE; }

	/** Called to inform the stats module of the chunk coords for newly processing chunk */
	virtual bool OnNewChunk(int a_ChunkX, int a_ChunkZ) = 0;

	/** Called to inform about the chunk's data offset in the file (chunk mini-header), the number of sectors it uses and the timestamp field value */
	virtual bool OnHeader(int a_FileOffset, unsigned char a_NumSectors, int a_Timestamp) { return CALLBACK_ABORT; }

	/** Called to inform of the compressed chunk data size and position in the file (offset from file start to the actual data) */
	virtual bool OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod) { return CALLBACK_ABORT; }

	/** Just in case you wanted to process the NBT yourself ;) */
	virtual bool OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize) { return CALLBACK_ABORT; }

	/** The chunk's NBT should specify chunk coords, these are sent here: */
	virtual bool OnRealCoords(int a_ChunkX, int a_ChunkZ) { return CALLBACK_ABORT; }

	/** The chunk contains a LastUpdate value specifying the last tick in which it was saved. */
	virtual bool OnLastUpdate(Int64 a_LastUpdate) { return CALLBACK_ABORT; }

	virtual bool OnTerrainPopulated(bool a_Populated) { return CALLBACK_ABORT; }

	virtual bool OnBiomes(const unsigned char * a_BiomeData) { return CALLBACK_ABORT; }

	/** Called when a heightmap for the chunk is read from the file.
	Note that the heightmap is given in big-endian ints, so if you want it, you need to ntohl() it first!
	*/
	virtual bool OnHeightMap(const int * a_HeightMapBE) { return CALLBACK_ABORT; }

	/** If there is data for the section, this callback is called; otherwise OnEmptySection() is called instead.
	All OnSection() callbacks are called first, and only then all the remaining sections are reported in OnEmptySection().
	*/
	virtual bool OnSection(
		unsigned char a_Y,
		const BLOCKTYPE * a_BlockTypes,
		const NIBBLETYPE * a_BlockAdditional,
		const NIBBLETYPE * a_BlockMeta,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_BlockSkyLight
	) { return CALLBACK_ABORT; }

	/** If there is no data for a section, this callback is called; otherwise OnSection() is called instead.
	OnEmptySection() callbacks are called after all OnSection() callbacks.
	*/
	virtual bool OnEmptySection(unsigned char a_Y) { return CALLBACK_CONTINUE; }

	/** Called after all sections have been processed via either OnSection() or OnEmptySection().
	*/
	virtual bool OnSectionsFinished(void) { return CALLBACK_ABORT; }

	/** Called for each entity in the chunk.
	Common parameters are parsed from the NBT.
	The callback may parse any other param from the a_NBT and a_NBTTag parameters.
	The a_NBTTag parameter points to the entity compound tag inside the Entities tag.
	*/
	virtual bool OnEntity(
		const AString & a_EntityType,
		double a_PosX, double a_PosY, double a_PosZ,
		double a_SpeedX, double a_SpeedY, double a_SpeedZ,
		float a_Yaw, float a_Pitch,
		float a_FallDistance,
		short a_FireTicksLeft,
		short a_AirTicks,
		char a_IsOnGround,
		cParsedNBT & a_NBT,
		int a_NBTTag
	) { return CALLBACK_ABORT; }

	/** Called for each tile entity in the chunk.
	Common parameters are parsed from the NBT.
	The callback may parse any other param from the a_NBT and a_NBTTag parameters.
	The a_NBTTag parameter points to the tile entity compound tag inside the TileEntities tag.
	*/
	virtual bool OnTileEntity(
		const AString & a_EntityType,
		int a_PosX, int a_PosY, int a_PosZ,
		cParsedNBT & a_NBT,
		int a_NBTTag
	) { return CALLBACK_ABORT; }

	/** Called for each tile tick in the chunk */
	virtual bool OnTileTick(
		int a_BlockType,
		int a_TicksLeft,
		int a_PosX, int a_PosY, int a_PosZ
	) { return CALLBACK_ABORT; }

	/** Called after the entire region file has been processed. No more callbacks for this region will be called. No  processing by default */
	virtual void OnRegionFinished(int a_RegionX, int a_RegionZ) {}
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

	/** Descendants override this method to return the correct callback type */
	virtual cCallback * CreateNewCallback(void) = 0;

	/** cProcessor uses this method to request a new callback */
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
