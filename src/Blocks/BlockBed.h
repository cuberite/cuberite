
// BlockBed.h

#pragma once

#include "BlockEntity.h"
#include "ChunkInterface.h"
#include "Entities/Player.h"
#include "Mixins/Mixins.h"





class cEntity;
class cWorldInterface;





class cBlockBedHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockBed(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::BlackBed:
			case BlockType::BlueBed:
			case BlockType::BrownBed:
			case BlockType::CyanBed:
			case BlockType::GrayBed:
			case BlockType::GreenBed:
			case BlockType::LightBlueBed:
			case BlockType::LightGrayBed:
			case BlockType::LimeBed:
			case BlockType::MagentaBed:
			case BlockType::OrangeBed:
			case BlockType::PinkBed:
			case BlockType::PurpleBed:
			case BlockType::RedBed:
			case BlockType::WhiteBed:
			case BlockType::YellowBed:
				return true;
			default: return false;
		}
	}

	static inline Vector3i BlockFaceToDirection(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_XM: return Vector3i(-1, 0,  0);
			case BLOCK_FACE_XP: return Vector3i( 1, 0,  0);
			case BLOCK_FACE_ZM: return Vector3i( 0, 0, -1);
			case BLOCK_FACE_ZP: return Vector3i( 0, 0,  1);
			default: return Vector3i();
		}
	}

	static inline eBlockFace GetBlockFace(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::BlackBed:     return BlackBed::Facing(a_Block);
			case BlockType::BlueBed:      return BlueBed::Facing(a_Block);
			case BlockType::BrownBed:     return BrownBed::Facing(a_Block);
			case BlockType::CyanBed:      return CyanBed::Facing(a_Block);
			case BlockType::GrayBed:      return GrayBed::Facing(a_Block);
			case BlockType::GreenBed:     return GreenBed::Facing(a_Block);
			case BlockType::LightBlueBed: return LightBlueBed::Facing(a_Block);
			case BlockType::LightGrayBed: return LightGrayBed::Facing(a_Block);
			case BlockType::LimeBed:      return LimeBed::Facing(a_Block);
			case BlockType::MagentaBed:   return MagentaBed::Facing(a_Block);
			case BlockType::OrangeBed:    return OrangeBed::Facing(a_Block);
			case BlockType::PinkBed:      return PinkBed::Facing(a_Block);
			case BlockType::PurpleBed:    return PurpleBed::Facing(a_Block);
			case BlockType::RedBed:       return RedBed::Facing(a_Block);
			case BlockType::WhiteBed:     return WhiteBed::Facing(a_Block);
			case BlockType::YellowBed:    return YellowBed::Facing(a_Block);
			default:                      return eBlockFace::BLOCK_FACE_NONE;
		}
	}

	static inline bool IsHeadPart(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::BlackBed:     return BlackBed::Part(a_Block)     == BlackBed::Part::Head;
			case BlockType::BlueBed:      return BlueBed::Part(a_Block)      == BlueBed::Part::Head;
			case BlockType::BrownBed:     return BrownBed::Part(a_Block)     == BrownBed::Part::Head;
			case BlockType::CyanBed:      return CyanBed::Part(a_Block)      == CyanBed::Part::Head;
			case BlockType::GrayBed:      return GrayBed::Part(a_Block)      == GrayBed::Part::Head;
			case BlockType::GreenBed:     return GreenBed::Part(a_Block)     == GreenBed::Part::Head;
			case BlockType::LightBlueBed: return LightBlueBed::Part(a_Block) == LightBlueBed::Part::Head;
			case BlockType::LightGrayBed: return LightGrayBed::Part(a_Block) == LightGrayBed::Part::Head;
			case BlockType::LimeBed:      return LimeBed::Part(a_Block)      == LimeBed::Part::Head;
			case BlockType::MagentaBed:   return MagentaBed::Part(a_Block)   == MagentaBed::Part::Head;
			case BlockType::OrangeBed:    return OrangeBed::Part(a_Block)    == OrangeBed::Part::Head;
			case BlockType::PinkBed:      return PinkBed::Part(a_Block)      == PinkBed::Part::Head;
			case BlockType::PurpleBed:    return PurpleBed::Part(a_Block)    == PurpleBed::Part::Head;
			case BlockType::RedBed:       return RedBed::Part(a_Block)       == RedBed::Part::Head;
			case BlockType::WhiteBed:     return WhiteBed::Part(a_Block)     == WhiteBed::Part::Head;
			case BlockType::YellowBed:    return YellowBed::Part(a_Block)    == YellowBed::Part::Head;
			default: return false;
		}
	}

	static inline bool IsBedOccupied(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::BlackBed:     return BlackBed::Occupied(a_Block);
			case BlockType::BlueBed:      return BlueBed::Occupied(a_Block);
			case BlockType::BrownBed:     return BrownBed::Occupied(a_Block);
			case BlockType::CyanBed:      return CyanBed::Occupied(a_Block);
			case BlockType::GrayBed:      return GrayBed::Occupied(a_Block);
			case BlockType::GreenBed:     return GreenBed::Occupied(a_Block);
			case BlockType::LightBlueBed: return LightBlueBed::Occupied(a_Block);
			case BlockType::LightGrayBed: return LightGrayBed::Occupied(a_Block);
			case BlockType::LimeBed:      return LimeBed::Occupied(a_Block);
			case BlockType::MagentaBed:   return MagentaBed::Occupied(a_Block);
			case BlockType::OrangeBed:    return OrangeBed::Occupied(a_Block);
			case BlockType::PinkBed:      return PinkBed::Occupied(a_Block);
			case BlockType::PurpleBed:    return PurpleBed::Occupied(a_Block);
			case BlockType::RedBed:       return RedBed::Occupied(a_Block);
			case BlockType::WhiteBed:     return WhiteBed::Occupied(a_Block);
			case BlockType::YellowBed:    return YellowBed::Occupied(a_Block);
			default: return false;
		}
	}

	static inline void SetBedOccupationState(cChunkInterface & a_ChunkInterface, Vector3i a_BedPosition, bool a_IsOccupied)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_BedPosition);
		switch (Self.Type())
		{
			case BlockType::BlackBed:     Self = BlackBed::BlackBed         (BlackBed::Facing(Self),     a_IsOccupied, BlackBed::Part(Self)); break;
			case BlockType::BlueBed:      Self = BlueBed::BlueBed           (BlueBed::Facing(Self),      a_IsOccupied, BlueBed::Part(Self)); break;
			case BlockType::BrownBed:     Self = BrownBed::BrownBed         (BrownBed::Facing(Self),     a_IsOccupied, BrownBed::Part(Self)); break;
			case BlockType::CyanBed:      Self = CyanBed::CyanBed           (CyanBed::Facing(Self),      a_IsOccupied, CyanBed::Part(Self)); break;
			case BlockType::GrayBed:      Self = GrayBed::GrayBed           (GrayBed::Facing(Self),      a_IsOccupied, GrayBed::Part(Self)); break;
			case BlockType::GreenBed:     Self = GreenBed::GreenBed         (GreenBed::Facing(Self),     a_IsOccupied, GreenBed::Part(Self)); break;
			case BlockType::LightBlueBed: Self = LightBlueBed::LightBlueBed (LightBlueBed::Facing(Self), a_IsOccupied, LightBlueBed::Part(Self)); break;
			case BlockType::LightGrayBed: Self = LightGrayBed::LightGrayBed (LightGrayBed::Facing(Self), a_IsOccupied, LightGrayBed::Part(Self)); break;
			case BlockType::LimeBed:      Self = LimeBed::LimeBed           (LimeBed::Facing(Self),      a_IsOccupied, LimeBed::Part(Self)); break;
			case BlockType::MagentaBed:   Self = MagentaBed::MagentaBed     (MagentaBed::Facing(Self),   a_IsOccupied, MagentaBed::Part(Self)); break;
			case BlockType::OrangeBed:    Self = OrangeBed::OrangeBed       (OrangeBed::Facing(Self),    a_IsOccupied, OrangeBed::Part(Self)); break;
			case BlockType::PinkBed:      Self = PinkBed::PinkBed           (PinkBed::Facing(Self),      a_IsOccupied, PinkBed::Part(Self)); break;
			case BlockType::PurpleBed:    Self = PurpleBed::PurpleBed       (PurpleBed::Facing(Self),    a_IsOccupied, PurpleBed::Part(Self)); break;
			case BlockType::RedBed:       Self = RedBed::RedBed             (RedBed::Facing(Self),       a_IsOccupied, RedBed::Part(Self)); break;
			case BlockType::WhiteBed:     Self = WhiteBed::WhiteBed         (WhiteBed::Facing(Self),     a_IsOccupied, WhiteBed::Part(Self)); break;
			case BlockType::YellowBed:    Self = YellowBed::YellowBed       (YellowBed::Facing(Self),    a_IsOccupied, YellowBed::Part(Self)); break;
			default: return;
		}
		a_ChunkInterface.SetBlock(a_BedPosition, Self);
	}

	static void VacateBed(cChunkInterface & a_ChunkInterface, cPlayer & a_Player)
	{
		auto BedPosition = a_Player.GetLastBedPos();

		auto BedBlock = a_ChunkInterface.GetBlock(BedPosition);

		if (!IsBlockBed(BedBlock))
		{
			// Bed was incomplete, just wake:
			a_Player.SetIsInBed(false);
			return;
		}

		if (!IsHeadPart(BedBlock))
		{
			// BedPosition is the foot of the bed, adjust to the head:
			BedPosition = AddFaceDirection(BedPosition, GetBlockFace(BedBlock));

			BedBlock = a_ChunkInterface.GetBlock(BedPosition);
			if (!IsBlockBed(BedBlock))
			{
				// Bed was incomplete, just wake:
				a_Player.SetIsInBed(false);
				return;
			}
		}

		// Clear the "occupied" bit of the bed's pillow block:
		SetBedOccupationState(a_ChunkInterface, BedPosition, false);

		// Wake the player:
		a_Player.SetIsInBed(false);
	}

private:

	// Overrides:
	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		const Vector3i a_BlockPos,
		BlockState a_OldBlock,
		const cEntity * a_Digger
	) const override;

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) const override;

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual bool IsUseable() const override
	{
		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 28;
	}
} ;
