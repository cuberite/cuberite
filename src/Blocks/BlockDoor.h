
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

	/** Returns a vector pointing one block in the direction the door is facing (where the outside is). */
	constexpr static Vector3i GetRelativeDirectionToOutside(NIBBLETYPE a_BlockMeta)
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
	constexpr static bool IsDoorBlockType(BLOCKTYPE a_Block)
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

#define IS_TOP(DoorType) DoorType::Half(a_Block) == DoorType::Half::Upper
	static bool IsTop(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaDoor:  return IS_TOP(AcaciaDoor);
			case BlockType::BirchDoor:   return IS_TOP(BirchDoor);
			case BlockType::CrimsonDoor: return IS_TOP(CrimsonDoor);
			case BlockType::DarkOakDoor: return IS_TOP(DarkOakDoor);
			case BlockType::JungleDoor:  return IS_TOP(JungleDoor);
			case BlockType::OakDoor:     return IS_TOP(OakDoor);
			case BlockType::SpruceDoor:  return IS_TOP(SpruceDoor);
			case BlockType::WarpedDoor:  return IS_TOP(WarpedDoor);
			case BlockType::IronDoor:    return IS_TOP(IronDoor);
			default: return false;
		}
	}
#undef IS_TOP

// Todo: Add this, once BlockState are implemented
/*
#define SET_OPEN(DoorType) \
	Block = DoorType::DoorType \
	( \
		DoorType::Facing(Block), \
		DoorType::Half(Block), \
		DoorType::Hinge(Block), \
		a_Open, \
		DoorType::Powered(Block)\
	); \
	break;                    \

	static void SetOpen(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos, bool a_Open)
	{
		using namespace Block;
		auto Block = a_ChunkInterface.GetBlock(a_BlockPos);
		switch (Block.Type())
		{
			case BlockType::AcaciaDoor:  SET_OPEN(AcaciaDoor);
			case BlockType::BirchDoor:   SET_OPEN(BirchDoor);
			case BlockType::CrimsonDoor: SET_OPEN(CrimsonDoor);
			case BlockType::DarkOakDoor: SET_OPEN(DarkOakDoor);
			case BlockType::JungleDoor:  SET_OPEN(JungleDoor);
			case BlockType::OakDoor:     SET_OPEN(OakDoor);
			case BlockType::SpruceDoor:  SET_OPEN(SpruceDoor);
			case BlockType::WarpedDoor:  SET_OPEN(WarpedDoor);
			case BlockType::IronDoor:    SET_OPEN(IronDoor);
			default: return;
		}
		a_ChunkInterface.SetBlock(a_BlockPos, Block);
	}
#undef SET_OPEN


#define SET_POWERED(DoorType) \
	DoorType::DoorType \
	( \
		DoorType::Facing(DoorBlock), \
		DoorType::Half(DoorBlock), \
		DoorType::Hinge(DoorBlock), \
		DoorType::Open(DoorBlock), \
		a_Powered \
	)

	static void SetPowered(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos, bool a_Powered)
	{
		using namespace Block;
		auto DoorBlock = a_ChunkInterface.GetBlock(a_BlockPos);
		switch (DoorBlock.Type())
		{
			case BlockType::AcaciaDoor:  DoorBlock = SET_POWERED(AcaciaDoor); break;
			case BlockType::BirchDoor:   DoorBlock = SET_POWERED(BirchDoor); break;
			case BlockType::CrimsonDoor: DoorBlock = SET_POWERED(CrimsonDoor); break;
			case BlockType::DarkOakDoor: DoorBlock = SET_POWERED(DarkOakDoor); break;
			case BlockType::JungleDoor:  DoorBlock = SET_POWERED(JungleDoor); break;
			case BlockType::OakDoor:     DoorBlock = SET_POWERED(OakDoor); break;
			case BlockType::SpruceDoor:  DoorBlock = SET_POWERED(SpruceDoor); break;
			case BlockType::WarpedDoor:  DoorBlock = SET_POWERED(WarpedDoor); break;
			case BlockType::IronDoor:    DoorBlock = SET_POWERED(IronDoor); break;
			default: return;
		}
		a_ChunkInterface.SetBlock(a_BlockPos, DoorBlock);
	}
*/

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
	virtual cBoundingBox GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) const override;

	// Todo: Mark as override once BlockState are implemented
	BlockState RotateCCW(BlockState a_Block) const;
	BlockState RotateCW(BlockState a_Block) const;
	BlockState MirrorXY(BlockState a_Block) const;
	BlockState MirrorYZ(BlockState a_Block) const;





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
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





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		// CanBeAt is also called on placement, so the top part can't check for the bottom part.
		// Both parts can only that their base is a valid block.

		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		const auto BasePosition = a_Position.addedY(((a_Meta & 0x8) == 0x8) ? -2 : -1);
		a_Chunk.GetBlockTypeMeta(BasePosition, BlockType, BlockMeta);

		return (BasePosition.y >= 0) && CanBeOn(BlockType, BlockMeta);
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




