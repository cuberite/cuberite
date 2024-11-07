
#pragma once

#include "BlockHandler.h"
#include "BlockType.h"
#include "Mixins/Mixins.h"
#include "Mixins/SolidSurfaceUnderneath.h"
#include "ChunkInterface.h"
#include "../Chunk.h"





class cBlockRedstoneRepeaterHandler final :
	public cSolidSurfaceUnderneath<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>
{
	using Super = cSolidSurfaceUnderneath<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>;

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





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_ITEM_REDSTONE_REPEATER, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




