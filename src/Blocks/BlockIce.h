
#pragma once

#include "BlockHandler.h"
#include "../Registries/BlockItemConverter.h"




class cBlockIceHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Only drop self when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(BlockItemConverter::FromBlock(m_BlockType));
		}

		return {};
	}

	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		// Disappears instantly in nether:
		if (a_WorldInterface.GetDimension() == dimNether)
		{
			a_Chunk.SetBlock(a_RelPos, Block::Air::Air());
			return;
		}

		// Artificial light on any of the surrounding block > 1 leads to melting the ice.
		static const std::array<Vector3i, 6> Adjacents
		{
			{
				{ 1, 0, 0 }, { -1, 0, 0 },
				{ 0, 1, 0 }, { 0, -1, 0 },
				{ 0, 0, 1 }, { 0, 0, -1 }
			}
		};

		for (const auto & Offset : Adjacents)
		{
			auto Position = a_RelPos + Offset;
			const auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(Position);

			if ((Chunk == nullptr) || !Chunk->IsValid())
			{
				continue;
			}

			if (!Chunk->IsLightValid())
			{
				Chunk->GetWorld()->QueueLightChunk(Chunk->GetPosX(), Chunk->GetPosZ());
				continue;
			}

			if (Chunk->GetBlockLight(Position) > 11)
			{
				a_Chunk.SetBlock(a_RelPos, Block::Water::Water());
				return;
			}
		}
	}

	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_OldBlock,
		const cEntity * a_Digger
	) const override
	{
		UNUSED(a_Digger);
		// If there's a solid block or a liquid underneath, convert to water, rather than air
		if (a_BlockPos.y <= 0)
		{
			return;
		}

		const auto Below = a_ChunkInterface.GetBlock(a_BlockPos.addedY(-1));
		if (cBlockInfo::FullyOccupiesVoxel(Below) || IsBlockLiquid(Below))
		{
			a_ChunkInterface.SetBlock(a_BlockPos, Block::Water::Water());
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 5;
	}
} ;
