// NBTChunkSerializer.h

#pragma once

#include "ChunkDef.h"



// fwd:
class cFastNBTWriter;
class cWorld;





/** Saves the chunk data into a NBT format, used by the Anvil storage.
The Writer is expected to be set up so that the serializer can write the chunk's top level "Level" NBT tag immediately.
Provides a single static entry point that does all the work, through a hidden worker class in the CPP file. */
class NBTChunkSerializer
{
public:

	/** Serializes the chunk into the specified writer.
	Returns true on success, false on failure (chunk not present etc.) */
	static bool serialize(const cWorld & aWorld, cChunkCoords aCoords, cFastNBTWriter & aWriter);
};
