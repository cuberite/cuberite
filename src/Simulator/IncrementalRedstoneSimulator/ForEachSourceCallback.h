
#pragma once

#include "RedstoneSimulatorChunkData.h"





class ForEachSourceCallback
{
public:

	ForEachSourceCallback(const cChunk & Chunk, Vector3i Position, BLOCKTYPE CurrentBlock);

	/** Callback invoked for each potential source position of the redstone component. */
	void operator()(Vector3i Location);

	/** Callback invoked for blocks supporting quasiconnectivity. */
	void CheckIndirectPower();

	// The maximum power level of all source locations.
	PowerLevel Power;

private:

	/** Returns whether a potential source position that's occupied by Block should be treated as linked. */
	static bool ShouldQueryLinkedPosition(BLOCKTYPE Block);

	/** Asks redstone handlers adjacent to a solid block how much power they will deliver to the querying position, via the solid block.
	Both QueryPosition and SolidBlockPosition are relative to Chunk. */
	static PowerLevel QueryLinkedPower(const cChunk & Chunk, Vector3i QueryPosition, BLOCKTYPE QueryBlock, Vector3i SolidBlockPosition);

	const cChunk & m_Chunk;
	const Vector3i m_Position;
	const BLOCKTYPE m_CurrentBlock;
};
