
#pragma once

#include "BlockHandler.h"





class cBlockIceHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Only drop self when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType);
		}
		else
		{
			return {};
		}
	}

	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		if (a_WorldInterface.GetDimension() == dimNether)
		{
			a_Chunk.SetBlock(a_RelPos, E_BLOCK_AIR, 0);
		}
		// Artificial light on any of the surrounding block > 11 leads to melting ice
		bool Smelt = false;
		Vector3i Pos;
		cChunk *Chunk;

		Vector3i Coords[7] =
			{{1, 0, 0}, {-1, 0, 0},
			{0, 1, 0}, {0, -1, 0},
			{0, 0, 1}, {0, 0, -1},
			{0, 0, 0}};

		auto StartAbsPos = a_Chunk.RelativeToAbsolute(a_RelPos);

		for (int i = 0; i < ARRAYCOUNT(Coords); ++i)
		{
			Pos = StartAbsPos + Coords[i];
			Chunk = a_Chunk.GetNeighborChunk(Pos.x, Pos.z);
			Smelt |= Chunk->GetBlockLight(Chunk->AbsoluteToRelative(Pos)) > 11;
		}

		if (Smelt)
		{
			a_Chunk.SetBlock(a_RelPos, E_BLOCK_STATIONARY_WATER, 0);
		}
	}

	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	) const override
	{
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
