
#pragma once

#include "BlockHandler.h"
#include "../BlockInfo.h"
#include "../Entities/Player.h"
#include "../Chunk.h"
#include "Mixins.h"
#include "ChunkInterface.h"
#include "BlockSlab.h"



class cBlockDoorHandler final :
	public cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>
{
	using Super = cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>;

public:

	using Super::Super;

	/** Returns true if door can be placed on the specified block type. */
	static bool CanBeOn(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		// Vanilla refuses to place doors on transparent blocks, except top-half slabs and other doors
		// We need to keep the door compatible with itself, otherwise the top half drops while the bottom half stays

		// Doors can be placed on upside-down slabs
		if (cBlockSlabHandler::IsAnySlabType(a_BlockType) && ((a_BlockMeta & 0x08) != 0))
		{
			return true;
		}
		// Doors can also be placed on other doors
		else if (IsDoorBlockType(a_BlockType))
		{
			return true;
		}
		// Doors can not be placed on transparent blocks, but on any other block
		else
		{
			return !cBlockInfo::IsTransparent(a_BlockType);
		}
	}

	static bool CanReplaceBlock(BLOCKTYPE a_BlockType)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_AIR:
			case E_BLOCK_TALL_GRASS:
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER:
			case E_BLOCK_LAVA:
			case E_BLOCK_STATIONARY_LAVA:
			case E_BLOCK_SNOW:
			case E_BLOCK_FIRE:
			{
				return true;
			}
		}
		return false;
	}

	/** Returns a vector pointing one block in the direction the door is facing (where the outside is). */
	inline static Vector3i GetRelativeDirectionToOutside(NIBBLETYPE a_BlockMeta)
	{
		switch (a_BlockMeta & 0x03)
		{
			case 0:  return Vector3i(-1, 0, 0);  // Facing West  / XM
			case 1:  return Vector3i(0, 0, -1);  // Facing North / ZM
			case 2:  return Vector3i(1, 0, 0);  // Facing East  / XP
			default: return Vector3i(0, 0, 1);  // Facing South / ZP
		}
	}

	/** Returns true if the specified blocktype is any kind of door */
	inline static bool IsDoorBlockType(BLOCKTYPE a_Block)
	{
		switch (a_Block)
		{
			case E_BLOCK_ACACIA_DOOR:
			case E_BLOCK_BIRCH_DOOR:
			case E_BLOCK_DARK_OAK_DOOR:
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_JUNGLE_DOOR:
			case E_BLOCK_SPRUCE_DOOR:
			case E_BLOCK_OAK_DOOR:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	/** Returns true iff the door at the specified coords is open.
	The coords may point to either the top part or the bottom part of the door. */
	static bool IsOpen(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos)
	{
		const auto Meta = GetCompleteDoorMeta(a_ChunkInterface, a_BlockPos);
		return (Meta & 0x04) != 0;
	}

	/** Sets the door to the specified state. If the door is already in that state, does nothing. */
	static void SetOpen(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos, bool a_Open)
	{
		BLOCKTYPE Block = a_ChunkInterface.GetBlock(a_BlockPos);
		if (!IsDoorBlockType(Block))
		{
			return;
		}

		NIBBLETYPE Meta = GetCompleteDoorMeta(a_ChunkInterface, a_BlockPos);
		bool IsOpened = ((Meta & 0x04) != 0);
		if (IsOpened == a_Open)
		{
			return;
		}

		// Change the door
		NIBBLETYPE NewMeta = (Meta & 0x07) ^ 0x04;  // Flip the "IsOpen" bit (0x04)
		if ((Meta & 0x08) == 0)
		{
			// The block is the bottom part of the door
			a_ChunkInterface.SetBlockMeta(a_BlockPos, NewMeta);
		}
		else
		{
			// The block is the top part of the door, set the meta to the corresponding top part
			if (a_BlockPos.y > 0)
			{
				a_ChunkInterface.SetBlockMeta(a_BlockPos.addedY(-1), NewMeta);
			}
		}
	}

private:

	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override;

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override;

	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) const override;

	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) const override;
	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta)  const override;
	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta)  const override;
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta)  const override;





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		// If clicking a bottom face, place the door one block lower:
		auto PlacedPos = a_PlacedBlockPos;
		if (a_ClickedBlockFace == BLOCK_FACE_BOTTOM)
		{
			PlacedPos.y--;
		}

		if (
			!CanReplaceBlock(a_ChunkInterface.GetBlock(PlacedPos)) ||
			!CanReplaceBlock(a_ChunkInterface.GetBlock(PlacedPos.addedY(1)))
		)
		{
			return false;
		}

		return Super::GetPlacementBlockTypeMeta(a_ChunkInterface, a_Player, PlacedPos, a_ClickedBlockFace, a_CursorPos, a_BlockType, a_BlockMeta);
	}

	virtual cBoundingBox GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) const override;





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		switch (m_BlockType)
		{
			case E_BLOCK_OAK_DOOR:      return cItem(E_ITEM_WOODEN_DOOR);
			case E_BLOCK_ACACIA_DOOR:   return cItem(E_ITEM_ACACIA_DOOR);
			case E_BLOCK_BIRCH_DOOR:    return cItem(E_ITEM_BIRCH_DOOR);
			case E_BLOCK_DARK_OAK_DOOR: return cItem(E_ITEM_DARK_OAK_DOOR);
			case E_BLOCK_JUNGLE_DOOR:   return cItem(E_ITEM_JUNGLE_DOOR);
			case E_BLOCK_SPRUCE_DOOR:   return cItem(E_ITEM_SPRUCE_DOOR);
			case E_BLOCK_IRON_DOOR:     return cItem(E_ITEM_IRON_DOOR);
			default:
			{
				ASSERT(!"Unhandled door type!");
				return {};
			}
		}
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		return ((a_RelPos.y > 0) && CanBeOn(a_Chunk.GetBlock(a_RelPos.addedY(-1)), a_Chunk.GetMeta(a_RelPos.addedY(-1))));
	}





	/** Returns the complete meta composed from the both parts of the door as (TopMeta << 4) | BottomMeta
	The coords may point to either part of the door.
	The returned value has bit 3 (0x08) set iff the coords point to the top part of the door.
	Fails silently for (invalid) doors on the world's top and bottom. */
	static NIBBLETYPE GetCompleteDoorMeta(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos)
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

		if ((Meta & 0x08) != 0)
		{
			// The coords are pointing at the top part of the door
			if (a_BlockPos.y > 0)
			{
				NIBBLETYPE DownMeta = a_ChunkInterface.GetBlockMeta(a_BlockPos.addedY(-1));
				return static_cast<NIBBLETYPE>((DownMeta & 0x07) | 0x08 | (Meta << 4));
			}
			// This is the top part of the door at the bottommost layer of the world, there's no bottom:
			return static_cast<NIBBLETYPE>(0x08 | (Meta << 4));
		}
		else
		{
			// The coords are pointing at the bottom part of the door
			if (a_BlockPos.y < cChunkDef::Height - 1)
			{
				NIBBLETYPE UpMeta = a_ChunkInterface.GetBlockMeta(a_BlockPos.addedY(1));
				return static_cast<NIBBLETYPE>(Meta | (UpMeta << 4));
			}
			// This is the bottom part of the door at the topmost layer of the world, there's no top:
			return Meta;
		}
	}





	/** Changes the door at the specified coords from open to close or vice versa */
	static void ChangeDoor(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos)
	{
		SetOpen(a_ChunkInterface, a_BlockPos, !IsOpen(a_ChunkInterface, a_BlockPos));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		switch (m_BlockType)
		{
			case E_BLOCK_OAK_DOOR: return 13;
			case E_BLOCK_SPRUCE_DOOR: return 34;
			case E_BLOCK_BIRCH_DOOR: return 2;
			case E_BLOCK_JUNGLE_DOOR: return 10;
			case E_BLOCK_DARK_OAK_DOOR: return 26;
			case E_BLOCK_ACACIA_DOOR: return 15;
			case E_BLOCK_IRON_DOOR: return 6;
			default:
			{
				ASSERT(!"Unhandled blocktype in door handler!");
				return 0;
			}
		}
	}
} ;




