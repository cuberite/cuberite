
#pragma once

#include "BlockHandler.h"
#include "BlockRedstoneRepeater.h"
#include "Mixins.h"





class cBlockComparatorHandler final :
	public cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>
{
	using Super = cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>;

public:

	using Super::Super;

	inline static bool IsInSubtractionMode(NIBBLETYPE a_Meta)
	{
		return ((a_Meta & 0x4) == 0x4);
	}

	inline static Vector3i GetFrontCoordinate(Vector3i a_Position, NIBBLETYPE a_Meta)
	{
		switch (a_Meta)
		{
			case 0x0: a_Position.z--; break;
			case 0x1: a_Position.x++; break;
			case 0x2: a_Position.z++; break;
			case 0x3: a_Position.x--; break;
			default:
			{
				LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
				ASSERT(!"Unknown metadata while determining orientation of comparator!");
				break;
			}
		}

		return a_Position;
	}

	inline static Vector3i GetSideCoordinate(Vector3i a_Position, NIBBLETYPE a_Meta, bool a_bInverse)
	{
		if (!a_bInverse)
		{
			switch (a_Meta)
			{
				case 0x0: a_Position.x++; break;
				case 0x1: a_Position.z--; break;
				case 0x2: a_Position.x--; break;
				case 0x3: a_Position.z++; break;
				default:
				{
					LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
					ASSERT(!"Unknown metadata while determining orientation of comparator!");
					break;
				}
			}
		}
		else
		{
			switch (a_Meta)
			{
				case 0x0: a_Position.x--; break;
				case 0x1: a_Position.z++; break;
				case 0x2: a_Position.x++; break;
				case 0x3: a_Position.z--; break;
				default:
				{
					LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
					ASSERT(!"Unknown metadata while determining orientation of comparator!");
					break;
				}
			}
		}

		return a_Position;
	}

	inline static Vector3i GetRearCoordinate(Vector3i a_Position, NIBBLETYPE a_Meta)
	{
		switch (a_Meta)
		{
			case 0x0: a_Position.z++; break;
			case 0x1: a_Position.x--; break;
			case 0x2: a_Position.z--; break;
			case 0x3: a_Position.x++; break;
			default:
			{
				LOGWARNING("%s: Unknown metadata: %d", __FUNCTION__, a_Meta);
				ASSERT(!"Unknown metadata while determining orientation of comparator!");
				break;
			}
		}

		return a_Position;
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
		const auto Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

		// Toggle the 3rd bit (addition / subtraction):
		a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta ^ 0x04);

		// Update simulators:
		a_WorldInterface.WakeUpSimulators(a_BlockPos);
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
		UNUSED(a_BlockFace);

		a_WorldInterface.WakeUpSimulators(a_BlockPos);
		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		const auto BelowPos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BelowPos))
		{
			return false;
		}

		BLOCKTYPE BelowBlock;
		NIBBLETYPE BelowBlockMeta;
		a_Chunk.GetBlockTypeMeta(BelowPos, BelowBlock, BelowBlockMeta);

		if (cBlockInfo::FullyOccupiesVoxel(BelowBlock))
		{
			return true;
		}

		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(BelowBlock))
		{
			return BelowBlockMeta & E_META_WOODEN_SLAB_UPSIDE_DOWN;
		}

		// upside down stairs
		if (cBlockStairsHandler::IsAnyStairType(BelowBlock))
		{
			return BelowBlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN;
		}

		return false;
	}





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_ITEM_COMPARATOR, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




