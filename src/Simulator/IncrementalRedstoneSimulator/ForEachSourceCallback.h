
#pragma once

#include "ForEachSourceCallback.h"
#include "RedstoneSimulatorChunkData.h"

class ForEachSourceCallback
{
public:

	ForEachSourceCallback(const cChunk & Chunk, Vector3i Position, BLOCKTYPE CurrentBlock);

	/** Returns whether a potential source position should be treated as linked. */
	bool ShouldQueryLinkedPosition(Vector3i Location, BLOCKTYPE Block);

	/** Callback invoked for each potential source position of the redstone component. */
	void operator()(Vector3i Location);

	/** Asks a redstone component at the source position how much power it will deliver to the querying position. */
	static PoweringData QueryPower(const cChunk & Chunk, Vector3i SourcePosition, BLOCKTYPE SourceBlock, Vector3i QueryPosition, BLOCKTYPE QueryBlock, bool IsLinked);

	/** Asks redstone handlers adjacent to a solid block how much power they will deliver to the querying position, via the solid block. */
	static PoweringData QueryLinkedPower(const cChunk & Chunk, Vector3i QueryPosition, BLOCKTYPE QueryBlock, Vector3i SolidBlockPosition);

	PoweringData Power;

private:

	const cChunk & m_Chunk;
	const Vector3i m_Position;
	const BLOCKTYPE m_CurrentBlock;
};
