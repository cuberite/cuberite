
#pragma once

#include "BlockHandler.h"
#include "BlockRedstoneRepeater.h"
#include "Mixins.h"





class cBlockComparatorHandler :
	public cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>
{
	using Super = cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>;

public:

	cBlockComparatorHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) override
	{
		// Toggle the 3rd bit (addition / subtraction):
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);
		Meta ^= 0x04;
		a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta);
		return true;
	}





	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) override
	{
		UNUSED(a_ChunkInterface);
		UNUSED(a_BlockFace);

		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual bool IsUseable(void) override
	{
		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_COMPARATOR, 1, 0);
	}





	inline static bool IsInSubtractionMode(NIBBLETYPE a_Meta)
	{
		return ((a_Meta & 0x4) == 0x4);
	}





	inline static bool IsOn(NIBBLETYPE a_Meta)
	{
		return ((a_Meta & 0x8) == 0x8);
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




