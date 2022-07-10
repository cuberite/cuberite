
// BlockTracer.h

// Declares the callback common to all blocktracers





#pragma once

#include "Defines.h"
#include "ChunkDef.h"





/** The callback class is used to notify the caller of individual events that are being traced. */
class BlockTracerCallbacks abstract
{
public:

	// Force a virtual destructor in descendants:
	virtual ~BlockTracerCallbacks() {}

	/** Called on each block encountered along the path, including the first block (path start)
	When this callback returns true, the tracing is aborted. */
	virtual bool OnNextBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) = 0;

	/** Called on each block encountered along the path, including the first block (path start), if chunk data is not loaded
	When this callback returns true, the tracing is aborted. */
	virtual bool OnNextBlockNoData(Vector3i a_BlockPos, eBlockFace a_EntryFace)
	{
		UNUSED(a_BlockPos);
		UNUSED(a_EntryFace);
		return false;
	}

	/** Called when the path goes out of world, either below (a_BlockPos.y < 0) or above (a_BlockPos.y >= cChunkDef::Height)
	The coords specify the exact point at which the path exited the world.
	If this callback returns true, the tracing is aborted.
	Note that some paths can go out of the world and come back again (parabola),
	in such a case this callback is followed by OnIntoWorld() and further OnNextBlock() calls. */
	virtual bool OnOutOfWorld(Vector3d a_BlockPos)
	{
		UNUSED(a_BlockPos);
		return false;
	}

	/** Called when the path goes into the world, from either below (a_BlockPos.y < 0) or above (a_BlockPos.y >= cChunkDef::Height)
	The coords specify the exact point at which the path entered the world.
	If this callback returns true, the tracing is aborted.
	Note that some paths can go out of the world and come back again (parabola),
	in such a case this callback is followed by further OnNextBlock() calls. */
	virtual bool OnIntoWorld(Vector3d a_BlockPos)
	{
		UNUSED(a_BlockPos);
		return false;
	}

	/** Called when the path is sure not to hit any more blocks.
	Note that for some shapes this might never happen (line with constant Y). */
	virtual void OnNoMoreHits(void) {}

	/** Called when the block tracing walks into a chunk that is not allocated.
	This usually means that the tracing is aborted. */
	virtual void OnNoChunk(void) {}
} ;
