
// ChunkStay.h

/* Declares the cChunkStay class representing a base for classes that want to wait for certain chunks to load,
then do some action on them. While the object is enabled, the chunks contained within are locked and will
not unload
*/





#pragma once

#include "ChunkDef.h"



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
class cChunkStay
{
public:
	cChunkStay(void);

	/** Deletes the object. Note that this calls Clear(), which means that the ChunkStay needs to be disabled. */
	virtual ~cChunkStay();

	/** Clears all the chunks that have been added.
	To be used only while the ChunkStay object is not enabled. */
	void Clear(void);

	/** Adds a chunk to be locked from unloading.
	To be used only while the ChunkStay object is not enabled. */
	void Add(int a_ChunkX, int a_ChunkZ);

	/** Releases the chunk so that it's no longer locked from unloading.
	To be used only while the ChunkStay object is not enabled. */
	void Remove(int a_ChunkX, int a_ChunkZ);

	/** Enables the ChunkStay on the specified chunkmap, causing it to load and generate chunks.
	All the contained chunks are queued for loading / generating. */
	void Enable (cChunkMap & a_ChunkMap);

	/** Disables the ChunkStay, the chunks are released and the ChunkStay object can be edited with Add() and Remove() again */
	virtual void Disable(void);

	/** Returns all the chunks that should be kept */
	const cChunkCoordsVector & GetChunks(void) const { return m_Chunks; }

	/** Called when a specific chunk become available. */
	virtual void OnChunkAvailable(int a_ChunkX, int a_ChunkZ) = 0;

	/** Called once all of the contained chunks are available.
	If returns true, the ChunkStay is automatically disabled by the ChunkMap; if it returns false, the ChunkStay is kept. */
	virtual bool OnAllChunksAvailable(void) = 0;

	/** Called by the ChunkMap when the ChunkStay is disabled. The object may choose to delete itself. */
	virtual void OnDisabled(void) = 0;

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
	May be called for chunks outside this ChunkStay.
	Returns true if the ChunkStay is to be automatically disabled by the ChunkMap; returns false to keep the ChunkStay. */
	bool ChunkAvailable(int a_ChunkX, int a_ChunkZ);
} ;





