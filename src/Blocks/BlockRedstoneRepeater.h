
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "ChunkInterface.h"
#include "BlockSlab.h"
#include "../Chunk.h"





class cBlockRedstoneRepeaterHandler final :
	public cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>
{
	using Super = cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>;

public:

	using Super::Super;

	inline static Vector3i GetFrontCoordinateOffset(NIBBLETYPE a_Meta)
	{
		return -GetRearCoordinateOffset(a_Meta);
	}

	inline static Vector3i GetLeftCoordinateOffset(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & E_META_REDSTONE_REPEATER_FACING_MASK)  // We only want the direction (bottom) bits
		{
			case E_META_REDSTONE_REPEATER_FACING_ZM: return { -1, 0, 0 };
			case E_META_REDSTONE_REPEATER_FACING_XP: return { 0, 0, -1 };
			case E_META_REDSTONE_REPEATER_FACING_ZP: return { 1, 0, 0 };
			case E_META_REDSTONE_REPEATER_FACING_XM: return { 0, 0, 1 };

			default:
			{
				LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
				ASSERT(!"Unknown metadata while determining orientation of repeater!");
				return { 0, 0, 0 };
			}
		}
	}

	inline static Vector3i GetRearCoordinateOffset(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & E_META_REDSTONE_REPEATER_FACING_MASK)  // We only want the direction (bottom) bits
		{
			case E_META_REDSTONE_REPEATER_FACING_ZM: return { 0, 0, 1 };
			case E_META_REDSTONE_REPEATER_FACING_XP: return { -1, 0, 0 };
			case E_META_REDSTONE_REPEATER_FACING_ZP: return { 0, 0, -1 };
			case E_META_REDSTONE_REPEATER_FACING_XM: return { 1, 0, 0 };
			default:
			{
				LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
				ASSERT(!"Unknown metadata while determining orientation of repeater!");
				return { 0, 0, 0 };
			}
		}
	}

private:

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		// Increment the delay setting:
		a_ChunkInterface.SetBlockMeta(a_BlockPos, ((a_ChunkInterface.GetBlockMeta(a_BlockPos) + 0x04) & 0x0f));
		return true;
	}





	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) const override
	{
		UNUSED(a_ChunkInterface);
		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock;
		NIBBLETYPE BelowBlockMeta;
		a_Chunk.GetBlockTypeMeta(a_RelPos.addedY(-1), BelowBlock, BelowBlockMeta);

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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(E_ITEM_REDSTONE_REPEATER, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




