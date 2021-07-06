
#pragma once

#include "BlockHandler.h"
#include "../BlockInfo.h"
#include "../Entities/Player.h"
#include "../Chunk.h"
#include "Mixins.h"
#include "ChunkInterface.h"
#include "BlockSlab.h"





class cBlockDoorHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	/** Returns true if door can be placed on the specified block type. */
	static bool CanBeOn(BlockState a_Block)
	{
		// Vanilla refuses to place doors on transparent blocks, except top-half slabs and other doors
		// We need to keep the door compatible with itself, otherwise the top half drops while the bottom half stays

		// Doors can be placed on upside-down slabs
		if (cBlockSlabHandler::IsAnySlabType(a_Block) && (cBlockSlabHandler::IsSlabTop(a_Block) || cBlockSlabHandler::IsSlabFull(a_Block)))
		{
			return true;
		}
		// Doors can also be placed on other doors
		else if (IsBlockDoor(a_Block))
		{
			return true;
		}
		// Doors can not be placed on transparent blocks, but on any other block
		else
		{
			return !cBlockInfo::IsTransparent(a_Block);
		}
	}

	/** Returns a vector pointing one block in the direction the door is facing (where the outside is). */
	inline static Vector3i GetRelativeDirectionToOutside(BlockState a_Block)
	{
		using namespace Block;
		eBlockFace Facing;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaDoor:
			{
				Facing = AcaciaDoor::Facing(a_Block);
				break;
			}
			case BlockType::BirchDoor:
			{
				Facing = BirchDoor::Facing(a_Block);
				break;
			}
			case BlockType::CrimsonDoor:
			{
				Facing = CrimsonDoor::Facing(a_Block);
				break;
			}
			case BlockType::DarkOakDoor:
			{
				Facing = DarkOakDoor::Facing(a_Block);
				break;
			}
			case BlockType::JungleDoor:
			{
				Facing = JungleDoor::Facing(a_Block);
				break;
			}
			case BlockType::OakDoor:
			{
				Facing = OakDoor::Facing(a_Block);
				break;
			}
			case BlockType::SpruceDoor:
			{
				Facing = SpruceDoor::Facing(a_Block);
				break;
			}
			case BlockType::WarpedDoor:
			{
				Facing = WarpedDoor::Facing(a_Block);
				break;
			}
			case BlockType::IronDoor:
			{
				Facing = IronDoor::Facing(a_Block);
				break;
			}
			default: return Vector3i();
		}

		switch (Facing)
		{
			case BLOCK_FACE_XM: return Vector3i(-1, 0, 0);
			case BLOCK_FACE_XP: return Vector3i(1, 0, 0);
			case BLOCK_FACE_ZM: return Vector3i(-1, 0, 0);
			case BLOCK_FACE_ZP: return Vector3i(0, 0, 1);
			default: return Vector3i();
		}
	}

	/** Returns true if the specified BlockType is any kind of door */
	inline static bool IsBlockDoor(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaDoor:
			case BlockType::BirchDoor:
			case BlockType::CrimsonDoor:
			case BlockType::DarkOakDoor:
			case BlockType::JungleDoor:
			case BlockType::OakDoor:
			case BlockType::SpruceDoor:
			case BlockType::WarpedDoor:
			case BlockType::IronDoor:
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
		using namespace Block;
		auto State = a_ChunkInterface.GetBlock(a_BlockPos);
		switch (State.Type())
		{
			case BlockType::AcaciaDoor:  return AcaciaDoor::Open(State);
			case BlockType::BirchDoor:   return BirchDoor::Open(State);
			case BlockType::CrimsonDoor: return CrimsonDoor::Open(State);
			case BlockType::DarkOakDoor: return DarkOakDoor::Open(State);
			case BlockType::JungleDoor:  return JungleDoor::Open(State);
			case BlockType::OakDoor:     return OakDoor::Open(State);
			case BlockType::SpruceDoor:  return SpruceDoor::Open(State);
			case BlockType::WarpedDoor:  return WarpedDoor::Open(State);
			case BlockType::IronDoor:    return IronDoor::Open(State);

			default: return false;
		}
	}

#define ISTOP(DoorType) \
	DoorType::Half(a_Block) == DoorType::Half::Upper

	static bool IsTop(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaDoor:  return ISTOP(AcaciaDoor);
			case BlockType::BirchDoor:   return ISTOP(BirchDoor);
			case BlockType::CrimsonDoor: return ISTOP(CrimsonDoor);
			case BlockType::DarkOakDoor: return ISTOP(DarkOakDoor);
			case BlockType::JungleDoor:  return ISTOP(JungleDoor);
			case BlockType::OakDoor:     return ISTOP(OakDoor);
			case BlockType::SpruceDoor:  return ISTOP(SpruceDoor);
			case BlockType::WarpedDoor:  return ISTOP(WarpedDoor);
			case BlockType::IronDoor:    return ISTOP(IronDoor);
			default: return false;
		}
	}

#define SETOPEN(DoorType) \
	Block = DoorType::DoorType \
	( \
		DoorType::Facing(Block), \
		DoorType::Half(Block), \
		DoorType::Hinge(Block), \
		a_Open, \
		DoorType::Powered(Block)\
	); \
	break;\

	/** Sets the door to the specified state. If the door is already in that state, does nothing. */
	static void SetOpen(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos, bool a_Open)
	{
		auto Block = a_ChunkInterface.GetBlock(a_BlockPos);
		if (!IsBlockDoor(Block))
		{
			return;
		}

		if (IsOpen(a_ChunkInterface, a_BlockPos) == a_Open)
		{
			return;
		}

		using namespace Block;

		switch (Block.Type())
		{
			case BlockType::AcaciaDoor:  SETOPEN(AcaciaDoor)
			case BlockType::BirchDoor:   SETOPEN(BirchDoor)
			case BlockType::CrimsonDoor: SETOPEN(CrimsonDoor)
			case BlockType::DarkOakDoor: SETOPEN(DarkOakDoor)
			case BlockType::JungleDoor:  SETOPEN(JungleDoor)
			case BlockType::OakDoor:     SETOPEN(OakDoor)
			case BlockType::SpruceDoor:  SETOPEN(SpruceDoor)
			case BlockType::WarpedDoor:  SETOPEN(WarpedDoor)
			case BlockType::IronDoor:    SETOPEN(IronDoor)
			default: return;
		}

		a_ChunkInterface.SetBlock(a_BlockPos, Block);

		if (IsTop(Block))
		{
			SetOpen(a_ChunkInterface, a_BlockPos.addedY(-1), a_Open);
		}
		else
		{
			SetOpen(a_ChunkInterface, a_BlockPos.addedY(1), a_Open);
		}
	}

#define SETPOWERED(DoorType) \
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
			case BlockType::AcaciaDoor:  DoorBlock = SETPOWERED(AcaciaDoor); break;
			case BlockType::BirchDoor:   DoorBlock = SETPOWERED(BirchDoor); break;
			case BlockType::CrimsonDoor: DoorBlock = SETPOWERED(CrimsonDoor); break;
			case BlockType::DarkOakDoor: DoorBlock = SETPOWERED(DarkOakDoor); break;
			case BlockType::JungleDoor:  DoorBlock = SETPOWERED(JungleDoor); break;
			case BlockType::OakDoor:     DoorBlock = SETPOWERED(OakDoor); break;
			case BlockType::SpruceDoor:  DoorBlock = SETPOWERED(SpruceDoor); break;
			case BlockType::WarpedDoor:  DoorBlock = SETPOWERED(WarpedDoor); break;
			case BlockType::IronDoor:    DoorBlock = SETPOWERED(IronDoor); break;
			default: return;
		}
		a_ChunkInterface.SetBlock(a_BlockPos, DoorBlock);
	}

private:

	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_Block,
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

	virtual BlockState RotateCCW(BlockState a_Block) const override;
	virtual BlockState RotateCW(BlockState a_Block)  const override;
	virtual BlockState MirrorXY(BlockState a_Block)  const override;
	virtual BlockState MirrorYZ(BlockState a_Block)  const override;

	virtual cBoundingBox GetPlacementCollisionBox(BlockState a_XM, BlockState a_XP, BlockState a_YM, BlockState a_YP, BlockState a_ZM, BlockState a_ZP) const override;





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaDoor:  return cItem(Item::AcaciaDoor);
			case BlockType::BirchDoor:   return cItem(Item::BirchDoor);
			case BlockType::CrimsonDoor: return cItem(Item::CrimsonDoor);
			case BlockType::DarkOakDoor: return cItem(Item::DarkOakDoor);
			case BlockType::JungleDoor:  return cItem(Item::JungleDoor);
			case BlockType::OakDoor:     return cItem(Item::OakDoor);
			case BlockType::SpruceDoor:  return cItem(Item::SpruceDoor);
			case BlockType::WarpedDoor:  return cItem(Item::WarpedDoor);
			case BlockType::IronDoor:    return cItem(Item::IronDoor);
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





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Self) const override
	{
		const auto BasePosition = a_Position.addedY(IsTop(a_Self.Type()) ? -2 : -1);
		auto BlockToReplace = a_Chunk.GetBlock(BasePosition);

		return (BasePosition.y >= 0) && CanBeOn(BlockToReplace);
	}





	/** Changes the door at the specified coords from open to close or vice versa */
	static void ChangeDoor(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos)
	{
		SetOpen(a_ChunkInterface, a_BlockPos, !IsOpen(a_ChunkInterface, a_BlockPos));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaDoor:  return 15;
			case BlockType::BirchDoor:   return 2;
			case BlockType::CrimsonDoor: return 35;
			case BlockType::DarkOakDoor: return 26;
			case BlockType::JungleDoor:  return 10;
			case BlockType::OakDoor:     return 13;
			case BlockType::SpruceDoor:  return 34;
			case BlockType::WarpedDoor:  return 56;
			case BlockType::IronDoor:    return 6;
			default:
			{
				ASSERT(!"Unhandled blocktype in door handler!");
				return 0;
			}
		}
	}
} ;




