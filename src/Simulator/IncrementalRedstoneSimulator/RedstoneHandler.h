
#pragma once

#include "RedstoneSimulatorChunkData.h"

class cChunk;
class ForEachSourceCallback;

namespace RedstoneHandler
{
	unsigned char GetPowerDeliveredToPosition(const cChunk & Chunk, Vector3i Position, BLOCKTYPE BlockType, Vector3i QueryPosition, BLOCKTYPE QueryBlockType, bool IsLinked);

	void Update(cChunk & Chunk, cChunk & CurrentlyTicking, Vector3i Position, BLOCKTYPE BlockType, NIBBLETYPE Meta, PoweringData PoweringData);

	void ForValidSourcePositions(const cChunk & Chunk, Vector3i Position, BLOCKTYPE BlockType, NIBBLETYPE Meta, ForEachSourceCallback & Callback);

	void SetWireState(const cChunk & Chunk, Vector3i Position);
}
