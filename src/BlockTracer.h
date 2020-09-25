
// BlockTracer.h

// Declares the classes common for all blocktracers





#pragma once





#include "Defines.h"
#include "ChunkDef.h"




// fwd: World.h
class cWorld;





class cBlockTracer abstract
{
public:
	/** The callback class is used to notify the caller of individual events that are being traced.
	*/
	class cCallbacks abstract
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cCallbacks() {}

		/** Called on each block encountered along the path, including the first block (path start)
		When this callback returns true, the tracing is aborted.
		*/
		virtual bool OnNextBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) = 0;

		/** Called on each block encountered along the path, including the first block (path start), if chunk data is not loaded
		When this callback returns true, the tracing is aborted.
		*/
		virtual bool OnNextBlockNoData(Vector3i a_BlockPos, char a_EntryFace)
		{
			UNUSED(a_BlockPos);
			UNUSED(a_EntryFace);
			return false;
		}

		/** Called when the path goes out of world, either below (a_BlockPos.y < 0) or above (a_BlockPos.y >= cChunkDef::Height)
		The coords specify the exact point at which the path exited the world.
		If this callback returns true, the tracing is aborted.
		Note that some paths can go out of the world and come back again (parabola),
		in such a case this callback is followed by OnIntoWorld() and further OnNextBlock() calls
		*/
		virtual bool OnOutOfWorld(Vector3d a_BlockPos)
		{
			UNUSED(a_BlockPos);
			return false;
		}

		/** Called when the path goes into the world, from either below (a_BlockPos.y < 0) or above (a_BlockPos.y >= cChunkDef::Height)
		The coords specify the exact point at which the path entered the world.
		If this callback returns true, the tracing is aborted.
		Note that some paths can go out of the world and come back again (parabola),
		in such a case this callback is followed by further OnNextBlock() calls
		*/
		virtual bool OnIntoWorld(Vector3d a_BlockPos)
		{
			UNUSED(a_BlockPos);
			return false;
		}

		/** Called when the path is sure not to hit any more blocks.
		Note that for some shapes this might never happen (line with constant Y)
		*/
		virtual void OnNoMoreHits(void) {}

		/** Called when the block tracing walks into a chunk that is not allocated.
		This usually means that the tracing is aborted.
		*/
		virtual void OnNoChunk(void) {}
	} ;


	/** Creates the BlockTracer parent with the specified callbacks */
	cBlockTracer(cWorld & a_World, cCallbacks & a_Callbacks) :
		m_World(&a_World),
		m_Callbacks(&a_Callbacks)
	{
	}


	/** Sets new world, returns the old one. Note that both need to be valid */
	cWorld & SetWorld(cWorld & a_World)
	{
		cWorld & Old = *m_World;
		m_World = &a_World;
		return Old;
	}


	/** Sets new callbacks, returns the old ones. Note that both need to be valid */
	cCallbacks & SetCallbacks(cCallbacks & a_NewCallbacks)
	{
		cCallbacks & Old = *m_Callbacks;
		m_Callbacks = &a_NewCallbacks;
		return Old;
	}

protected:
	/** The world upon which to operate */
	cWorld * m_World;

	/** The callback to use for reporting */
	cCallbacks * m_Callbacks;
} ;




