
#pragma once

#include "BlockHandler.h"





class cBlockIceHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Only drop self when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType);
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
			a_Chunk.SetBlock(a_RelPos, E_BLOCK_AIR, 0);
			return;
		}

		// Artificial light on any of the surrounding block > 11 leads to melting the ice.
		static const std::array<Vector3i, 7> Adjacents
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
				a_Chunk.SetBlock(a_RelPos, E_BLOCK_STATIONARY_WATER, 0);
				return;
			}
		}
	}

	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
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
			a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_WATER, 0);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 5;
	}
} ;
