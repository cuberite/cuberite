
#pragma once

#include "BlockHandler.h"





class cBlockSpongeHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual void OnPlaced(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_Block
	) const override
	{
		OnNeighborChanged(a_ChunkInterface, a_BlockPos, BLOCK_FACE_NONE);
	}

	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk) { CheckSoaked(cChunkDef::AbsoluteToRelative(a_BlockPos), a_Chunk); return true; });
	}

	/** Check blocks around the sponge to see if they are water.
	If a dry sponge is touching water, soak up up to 65 blocks of water,
	with a taxicab distance of 7, and turn the sponge into a wet sponge. */
	static void CheckSoaked(Vector3i a_Rel, cChunk & a_Chunk)
	{
		struct sSeed
		{
			sSeed(Vector3i pos, int d)
			{
				m_Pos = pos;
				m_Depth = d;
			}
			Vector3i m_Pos;
			int m_Depth;
		};

		// Check if this is a dry sponge next to a water block.
		BlockState Target = a_Chunk.GetBlock(a_Rel.x, a_Rel.y, a_Rel.z);
		if (Target.Type() != BlockType::Sponge)
		{
			return;
		}

		const auto & WaterCheck = cSimulator::ThreeDimensionalNeighborCoords;
		const bool ShouldSoak = std::any_of(WaterCheck.cbegin(), WaterCheck.cend(), [a_Rel, &a_Chunk](Vector3i a_Offset)
		{
			return IsWet(a_Rel + a_Offset, a_Chunk);
		});

		// Early return if the sponge isn't touching any water.
		if (!ShouldSoak)
		{
			return;
		}

		// Use a queue to hold blocks that we want to check, so our search is breadth-first.
		std::queue<sSeed> Seeds;
		int count = 0;
		// Only go 7 blocks away from the center block.
		const int maxDepth = 7;
		// Start with the 6 blocks around the sponge.
		for (unsigned int i = 0; i < 6; i++)
		{
			Seeds.emplace(a_Rel + WaterCheck[i], maxDepth - 1);
		}

		// Keep checking blocks that are touching water blocks, or until 65 have been soaked up.
		while (!Seeds.empty() && count < 65)
		{
			sSeed seed = Seeds.front();
			Vector3i checkRel = seed.m_Pos;
			if (IsWet(checkRel, a_Chunk))
			{
				count++;
				DryUp(checkRel, a_Chunk);
				if (seed.m_Depth > 0)
				{
					// If this block was water, and we haven't yet gone too far away,
					// add its neighbors to the queue to check.
					for (unsigned int i = 0; i < 6; i++)
					{
						Seeds.emplace(checkRel + WaterCheck[i], seed.m_Depth - 1);
					}
				}
			}
			Seeds.pop();
		}

		a_Chunk.SetBlock(a_Rel, Block::WetSponge::WetSponge());
	}

	static void DryUp(Vector3i a_Rel, cChunk & a_Chunk)
	{
		// TODO: support evaporating waterlogged blocks.
		a_Chunk.UnboundedRelSetBlock(a_Rel.x, a_Rel.y, a_Rel.z, Block::Air::Air());
	}

	static bool IsWet(Vector3i a_Rel, cChunk & a_Chunk)
	{
		// TODO: support detecting waterlogged blocks.
		BlockState Block;
		return(a_Chunk.UnboundedRelGetBlock({a_Rel.x, a_Rel.y, a_Rel.z}, Block) && Block.Type() == BlockType::Water);
	}

	virtual ColourID GetMapBaseColourID() const override
	{
		return 18;
	}
};
