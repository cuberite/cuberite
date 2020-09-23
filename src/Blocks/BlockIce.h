
#pragma once

#include "BlockHandler.h"





class cBlockIceHandler :
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
		else
		{
			return {};
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
