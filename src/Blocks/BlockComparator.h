
#pragma once

#include "BlockHandler.h"
#include "BlockRedstoneRepeater.h"
#include "Mixins.h"





class cBlockComparatorHandler :
	public cMetaRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03, true>
{
	using super = cMetaRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03, true>;

public:

	cBlockComparatorHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta({a_BlockX, a_BlockY, a_BlockZ});
		Meta ^= 0x04;  // Toggle 3rd (addition / subtraction) bit with XOR
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
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
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR));
	}

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_COMPARATOR, 1, 0);
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
		) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = cBlockRedstoneRepeaterHandler::RepeaterRotationToMetaData(a_Player.GetYaw());
		return true;
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




