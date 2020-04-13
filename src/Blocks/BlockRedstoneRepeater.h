
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "ChunkInterface.h"
#include "BlockSlab.h"
#include "../Chunk.h"





class cBlockRedstoneRepeaterHandler:
	public cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>
{
	using Super = cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>;

public:

	cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, ((a_ChunkInterface.GetBlockMeta({a_BlockX, a_BlockY, a_BlockZ}) + 0x04) & 0x0f));
		return true;
	}

	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		UNUSED(a_ChunkInterface);
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock;
		NIBBLETYPE BelowBlockMeta;
		a_Chunk.GetBlockTypeMeta(a_RelX, a_RelY - 1, a_RelZ, BelowBlock, BelowBlockMeta);

		if (cBlockInfo::FullyOccupiesVoxel(BelowBlock))
		{
			return true;
		}
		else if (cBlockSlabHandler::IsAnySlabType(BelowBlock))
		{
			// Check if the slab is turned up side down
			if ((BelowBlockMeta & 0x08) == 0x08)
			{
				return true;
			}
		}
		return false;
	}

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_REDSTONE_REPEATER, 1, 0);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}


	inline static Vector3i GetRearCoordinateOffset(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x3)  // We only want the direction (bottom) bits
		{
			case 0x0: return {0, 0, 1};
			case 0x1: return {-1, 0, 0};
			case 0x2: return {0, 0, -1};
			case 0x3: return {1, 0, 0};
			default:
			{
				LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
				ASSERT(!"Unknown metadata while determining orientation of repeater!");
				return {0, 0, 0};
			}
		}
	}


	inline static Vector3i GetFrontCoordinateOffset(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x3)  // We only want the direction (bottom) bits
		{
			case 0x0: return {0, 0, -1};
			case 0x1: return {1, 0, 0};
			case 0x2: return {0, 0, 1};
			case 0x3: return {-1, 0, 0};
			default:
			{
				LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
				ASSERT(!"Unknown metadata while determining orientation of repeater!");
				return {0, 0, 0};
			}
		}
	}
} ;




