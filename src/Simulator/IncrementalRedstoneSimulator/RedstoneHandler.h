
#pragma once

#include "RedstoneSimulatorChunkData.h"





class cChunk;
class ForEachSourceCallback;





namespace RedstoneHandler
{
	/** Asks a redstone component at the source position how much power it will deliver to the querying position.
	If IsLinked is true, QueryPosition should point to the intermediate conduit block.
	The Position and QueryPosition are both relative to Chunk. */
	PowerLevel GetPowerDeliveredToPosition(const cChunk & Chunk, Vector3i Position, BLOCKTYPE BlockType, Vector3i QueryPosition, BLOCKTYPE QueryBlockType, bool IsLinked);

	/** Tells a redstone component at this position to update itself.
	PowerLevel represents the maximum power level all of its source positions gave to it.
	Position is relative to Chunk, but if the component needs to queue neighbour updates, they are queued to CurrentlyTicking. */
	void Update(cChunk & Chunk, cChunk & CurrentlyTicking, Vector3i Position, BLOCKTYPE BlockType, NIBBLETYPE Meta, PowerLevel PowerLevel);

	/** Invokes Callback for each position this component can accept power from. */
	void ForValidSourcePositions(const cChunk & Chunk, Vector3i Position, BLOCKTYPE BlockType, NIBBLETYPE Meta, ForEachSourceCallback & Callback);

	/** Temporary: compute and set the block state of a redstone wire. */
	void SetWireState(const cChunk & Chunk, Vector3i Position);
}
