
// ChunkStay.h

/* Declares the cChunkStay class representing a base for classes that want to wait for certain chunks to load,
then do some action on them. While the object is enabled, the chunks contained within are locked and will
not unload
*/





#pragma once





// fwd
class cChunkMap;





/** Makes chunks stay loaded until this object is cleared or destroyed
Works by setting internal flags in the cChunk that it should not be unloaded.
To optimize for speed, cChunkStay has an Enabled flag, it will "stay" the chunks only when enabled
and it will refuse chunk-list manipulations when enabled.
The object itself is not made thread-safe, it's supposed to be used from a single thread only.
This class is abstract, the descendants are expected to provide the OnChunkAvailable() and
the OnAllChunksAvailable() callback implementations. Note that those are called from the contexts of
different threads' - the caller, the Loader or the Generator thread.
*/
// tolua_begin
class cChunkStay
{
public:
	// tolua_end
	cChunkStay(void);
	~cChunkStay();
	
	// tolua_begin
	
	void Clear(void);
	
	/** Adds a chunk to be locked from unloading.
	To be used only while the ChunkStay object is not enabled. */
	void Add   (int a_ChunkX, int a_ChunkZ);
	
	/** Releases the chunk so that it's no longer locked from unloading.
	To be used only while the ChunkStay object is not enabled. */
	void Remove(int a_ChunkX, int a_ChunkZ);
	
	// tolua_end
	
	/** Enables the ChunkStay on the specified chunkmap, causing it to load and generate chunks.
	All the contained chunks are queued for loading / generating. */
	void Enable (cChunkMap & a_ChunkMap);
	
	// tolua_begin
	
	/** Disables the ChunkStay, the chunks are released and the ChunkStay
	object can be edited with Add() and Remove() again*/
	void Disable(void);
	
	// tolua_end
	
	/** Returns all the chunks that should be kept */
	const cChunkCoordsVector & GetChunks(void) const { return m_Chunks; }
	
	/** Called when a specific chunk become available. */
	virtual void OnChunkAvailable(int a_ChunkX, int a_ChunkZ) = 0;
	
	/** Caled once all of the contained chunks are available. */
	virtual void OnAllChunksAvailable(void) = 0;
	
protected:

	friend class cChunkMap;


	/** The chunkmap where the object is enabled.
	Valid only after call to Enable() and before Disable(). */
	cChunkMap * m_ChunkMap;
	
	/** The list of chunks to lock from unloading. */
	cChunkCoordsVector m_Chunks;
	
	/** The chunks that still need loading */
	cChunkCoordsVector m_OutstandingChunks;
	
	
	/** Called by cChunkMap when a chunk is available, checks m_NumLoaded and triggers the appropriate callbacks.
	May be called for chunks outside this ChunkStay. */
	void ChunkAvailable(int a_ChunkX, int a_ChunkZ);
} ;  // tolua_export





