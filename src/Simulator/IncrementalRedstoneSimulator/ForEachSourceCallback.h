
#pragma once

#include "RedstoneSimulatorChunkData.h"

class ForEachSourceCallback
{
public:

	ForEachSourceCallback(const cChunk & Chunk, Vector3i Position, BLOCKTYPE CurrentBlock);

	/** Returns whether a potential source position should be treated as linked. */
	bool ShouldQueryLinkedPosition(Vector3i Location, BLOCKTYPE Block);

	/** Callback invoked for each potential source position of the redstone component. */
	void operator()(Vector3i Location);

	/** Asks redstone handlers adjacent to a solid block how much power they will deliver to the querying position, via the solid block.
	Both QueryPosition and SolidBlockPosition are relative to Chunk. */
	static PowerLevel QueryLinkedPower(const cChunk & Chunk, Vector3i QueryPosition, BLOCKTYPE QueryBlock, Vector3i SolidBlockPosition);

	PowerLevel Power;

private:

	const cChunk & m_Chunk;
	const Vector3i m_Position;
	const BLOCKTYPE m_CurrentBlock;
};
