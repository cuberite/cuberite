
// BlockBed.h

#pragma once

#include "BlockEntity.h"
#include "ChunkInterface.h"
#include "Entities/Player.h"
#include "Mixins.h"





class cEntity;
class cWorldInterface;





class cBlockBedHandler final :
	public cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>
{
	using Super = cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>;

public:

	using Super::Super;

	static constexpr bool IsBlockBed(BlockState a_Block)
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

	static constexpr Vector3i BlockFaceToDirection(eBlockFace a_BlockFace)
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

	static constexpr eBlockFace GetRotation(BlockState a_Block)
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


	static constexpr bool IsHeadPart(BlockState a_Block)
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

	static constexpr bool IsBedOccupied(BlockState a_Block)
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
		// TODO: This is a temporary workaround until we have a proper blockstate system - 12xx12
		// auto Self = a_ChunkInterface.GetBlock(a_BedPosition);
		auto Self = BlackBed::BlackBed();
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
		// TODO: See above - 12xx12
		// a_ChunkInterface.SetBlock(a_BedPosition, Self);
	}

	/** Rotates a given block counter-clockwise. */
	BlockState RotateCCW(BlockState a_Block) const
	{
		using namespace Block;
		auto Rotation = RotateBlockFaceCCW(GetRotation(a_Block));

		switch (a_Block.Type())
		{
			case BlockType::BlackBed:     return BlackBed::BlackBed         (Rotation, BlackBed::Occupied(a_Block), BlackBed::Part(a_Block));
			case BlockType::BlueBed:      return BlueBed::BlueBed           (Rotation, BlueBed::Occupied(a_Block), BlueBed::Part(a_Block));
			case BlockType::BrownBed:     return BrownBed::BrownBed         (Rotation, BrownBed::Occupied(a_Block), BrownBed::Part(a_Block));
			case BlockType::CyanBed:      return CyanBed::CyanBed           (Rotation, CyanBed::Occupied(a_Block), CyanBed::Part(a_Block));
			case BlockType::GrayBed:      return GrayBed::GrayBed           (Rotation, GrayBed::Occupied(a_Block), GrayBed::Part(a_Block));
			case BlockType::GreenBed:     return GreenBed::GreenBed         (Rotation, GreenBed::Occupied(a_Block), GreenBed::Part(a_Block));
			case BlockType::LightBlueBed: return LightBlueBed::LightBlueBed (Rotation, LightBlueBed::Occupied(a_Block), LightBlueBed::Part(a_Block));
			case BlockType::LightGrayBed: return LightGrayBed::LightGrayBed (Rotation, LightGrayBed::Occupied(a_Block), LightGrayBed::Part(a_Block));
			case BlockType::LimeBed:      return LimeBed::LimeBed           (Rotation, LimeBed::Occupied(a_Block), LimeBed::Part(a_Block));
			case BlockType::MagentaBed:   return MagentaBed::MagentaBed     (Rotation, MagentaBed::Occupied(a_Block), MagentaBed::Part(a_Block));
			case BlockType::OrangeBed:    return OrangeBed::OrangeBed       (Rotation, OrangeBed::Occupied(a_Block), OrangeBed::Part(a_Block));
			case BlockType::PinkBed:      return PinkBed::PinkBed           (Rotation, PinkBed::Occupied(a_Block), PinkBed::Part(a_Block));
			case BlockType::PurpleBed:    return PurpleBed::PurpleBed       (Rotation, PurpleBed::Occupied(a_Block), PurpleBed::Part(a_Block));
			case BlockType::RedBed:       return RedBed::RedBed             (Rotation, RedBed::Occupied(a_Block), RedBed::Part(a_Block));
			case BlockType::WhiteBed:     return WhiteBed::WhiteBed         (Rotation, WhiteBed::Occupied(a_Block), WhiteBed::Part(a_Block));
			case BlockType::YellowBed:    return YellowBed::YellowBed       (Rotation, YellowBed::Occupied(a_Block), YellowBed::Part(a_Block));
			default: return a_Block;
		}
	}

	/** Rotates a given block clockwise. */
	BlockState RotateCW(BlockState a_Block) const
	{
		using namespace Block;
		auto Rotation = RotateBlockFaceCW(GetRotation(a_Block));

		switch (a_Block.Type())
		{
			case BlockType::BlackBed:     return BlackBed::BlackBed         (Rotation, BlackBed::Occupied(a_Block), BlackBed::Part(a_Block));
			case BlockType::BlueBed:      return BlueBed::BlueBed           (Rotation, BlueBed::Occupied(a_Block), BlueBed::Part(a_Block));
			case BlockType::BrownBed:     return BrownBed::BrownBed         (Rotation, BrownBed::Occupied(a_Block), BrownBed::Part(a_Block));
			case BlockType::CyanBed:      return CyanBed::CyanBed           (Rotation, CyanBed::Occupied(a_Block), CyanBed::Part(a_Block));
			case BlockType::GrayBed:      return GrayBed::GrayBed           (Rotation, GrayBed::Occupied(a_Block), GrayBed::Part(a_Block));
			case BlockType::GreenBed:     return GreenBed::GreenBed         (Rotation, GreenBed::Occupied(a_Block), GreenBed::Part(a_Block));
			case BlockType::LightBlueBed: return LightBlueBed::LightBlueBed (Rotation, LightBlueBed::Occupied(a_Block), LightBlueBed::Part(a_Block));
			case BlockType::LightGrayBed: return LightGrayBed::LightGrayBed (Rotation, LightGrayBed::Occupied(a_Block), LightGrayBed::Part(a_Block));
			case BlockType::LimeBed:      return LimeBed::LimeBed           (Rotation, LimeBed::Occupied(a_Block), LimeBed::Part(a_Block));
			case BlockType::MagentaBed:   return MagentaBed::MagentaBed     (Rotation, MagentaBed::Occupied(a_Block), MagentaBed::Part(a_Block));
			case BlockType::OrangeBed:    return OrangeBed::OrangeBed       (Rotation, OrangeBed::Occupied(a_Block), OrangeBed::Part(a_Block));
			case BlockType::PinkBed:      return PinkBed::PinkBed           (Rotation, PinkBed::Occupied(a_Block), PinkBed::Part(a_Block));
			case BlockType::PurpleBed:    return PurpleBed::PurpleBed       (Rotation, PurpleBed::Occupied(a_Block), PurpleBed::Part(a_Block));
			case BlockType::RedBed:       return RedBed::RedBed             (Rotation, RedBed::Occupied(a_Block), RedBed::Part(a_Block));
			case BlockType::WhiteBed:     return WhiteBed::WhiteBed         (Rotation, WhiteBed::Occupied(a_Block), WhiteBed::Part(a_Block));
			case BlockType::YellowBed:    return YellowBed::YellowBed       (Rotation, YellowBed::Occupied(a_Block), YellowBed::Part(a_Block));
			default: return a_Block;
		}
	}

	/** Mirrors a given block meta around the XY plane. */
	BlockState MirrorXY(BlockState a_Block) const
	{
		using namespace Block;
		auto Rotation = MirrorBlockFaceXY(GetRotation(a_Block));

		switch (a_Block.Type())
		{
			case BlockType::BlackBed:     return BlackBed::BlackBed         (Rotation, BlackBed::Occupied(a_Block), BlackBed::Part(a_Block));
			case BlockType::BlueBed:      return BlueBed::BlueBed           (Rotation, BlueBed::Occupied(a_Block), BlueBed::Part(a_Block));
			case BlockType::BrownBed:     return BrownBed::BrownBed         (Rotation, BrownBed::Occupied(a_Block), BrownBed::Part(a_Block));
			case BlockType::CyanBed:      return CyanBed::CyanBed           (Rotation, CyanBed::Occupied(a_Block), CyanBed::Part(a_Block));
			case BlockType::GrayBed:      return GrayBed::GrayBed           (Rotation, GrayBed::Occupied(a_Block), GrayBed::Part(a_Block));
			case BlockType::GreenBed:     return GreenBed::GreenBed         (Rotation, GreenBed::Occupied(a_Block), GreenBed::Part(a_Block));
			case BlockType::LightBlueBed: return LightBlueBed::LightBlueBed (Rotation, LightBlueBed::Occupied(a_Block), LightBlueBed::Part(a_Block));
			case BlockType::LightGrayBed: return LightGrayBed::LightGrayBed (Rotation, LightGrayBed::Occupied(a_Block), LightGrayBed::Part(a_Block));
			case BlockType::LimeBed:      return LimeBed::LimeBed           (Rotation, LimeBed::Occupied(a_Block), LimeBed::Part(a_Block));
			case BlockType::MagentaBed:   return MagentaBed::MagentaBed     (Rotation, MagentaBed::Occupied(a_Block), MagentaBed::Part(a_Block));
			case BlockType::OrangeBed:    return OrangeBed::OrangeBed       (Rotation, OrangeBed::Occupied(a_Block), OrangeBed::Part(a_Block));
			case BlockType::PinkBed:      return PinkBed::PinkBed           (Rotation, PinkBed::Occupied(a_Block), PinkBed::Part(a_Block));
			case BlockType::PurpleBed:    return PurpleBed::PurpleBed       (Rotation, PurpleBed::Occupied(a_Block), PurpleBed::Part(a_Block));
			case BlockType::RedBed:       return RedBed::RedBed             (Rotation, RedBed::Occupied(a_Block), RedBed::Part(a_Block));
			case BlockType::WhiteBed:     return WhiteBed::WhiteBed         (Rotation, WhiteBed::Occupied(a_Block), WhiteBed::Part(a_Block));
			case BlockType::YellowBed:    return YellowBed::YellowBed       (Rotation, YellowBed::Occupied(a_Block), YellowBed::Part(a_Block));
			default: return a_Block;
		}
	}

	/** Mirrors a given block meta around the YZ plane. */
	BlockState MirrorYZ(BlockState a_Block) const
	{
		using namespace Block;
		auto Rotation = MirrorBlockFaceYZ(GetRotation(a_Block));

		switch (a_Block.Type())
		{
			case BlockType::BlackBed:     return BlackBed::BlackBed         (Rotation, BlackBed::Occupied(a_Block), BlackBed::Part(a_Block));
			case BlockType::BlueBed:      return BlueBed::BlueBed           (Rotation, BlueBed::Occupied(a_Block), BlueBed::Part(a_Block));
			case BlockType::BrownBed:     return BrownBed::BrownBed         (Rotation, BrownBed::Occupied(a_Block), BrownBed::Part(a_Block));
			case BlockType::CyanBed:      return CyanBed::CyanBed           (Rotation, CyanBed::Occupied(a_Block), CyanBed::Part(a_Block));
			case BlockType::GrayBed:      return GrayBed::GrayBed           (Rotation, GrayBed::Occupied(a_Block), GrayBed::Part(a_Block));
			case BlockType::GreenBed:     return GreenBed::GreenBed         (Rotation, GreenBed::Occupied(a_Block), GreenBed::Part(a_Block));
			case BlockType::LightBlueBed: return LightBlueBed::LightBlueBed (Rotation, LightBlueBed::Occupied(a_Block), LightBlueBed::Part(a_Block));
			case BlockType::LightGrayBed: return LightGrayBed::LightGrayBed (Rotation, LightGrayBed::Occupied(a_Block), LightGrayBed::Part(a_Block));
			case BlockType::LimeBed:      return LimeBed::LimeBed           (Rotation, LimeBed::Occupied(a_Block), LimeBed::Part(a_Block));
			case BlockType::MagentaBed:   return MagentaBed::MagentaBed     (Rotation, MagentaBed::Occupied(a_Block), MagentaBed::Part(a_Block));
			case BlockType::OrangeBed:    return OrangeBed::OrangeBed       (Rotation, OrangeBed::Occupied(a_Block), OrangeBed::Part(a_Block));
			case BlockType::PinkBed:      return PinkBed::PinkBed           (Rotation, PinkBed::Occupied(a_Block), PinkBed::Part(a_Block));
			case BlockType::PurpleBed:    return PurpleBed::PurpleBed	    (Rotation, PurpleBed::Occupied(a_Block), PurpleBed::Part(a_Block));
			case BlockType::RedBed:       return RedBed::RedBed             (Rotation, RedBed::Occupied(a_Block), RedBed::Part(a_Block));
			case BlockType::WhiteBed:     return WhiteBed::WhiteBed         (Rotation, WhiteBed::Occupied(a_Block), WhiteBed::Part(a_Block));
			case BlockType::YellowBed:    return YellowBed::YellowBed       (Rotation, YellowBed::Occupied(a_Block), YellowBed::Part(a_Block));
			default: return a_Block;

		}
	}

	static Vector3i MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0: return Vector3i(0, 0, 1);
			case 1: return Vector3i(-1, 0, 0);
			case 2: return Vector3i(0, 0, -1);
			case 3: return Vector3i(1, 0, 0);
		}
		return Vector3i();
	}

	static void VacateBed(cChunkInterface & a_ChunkInterface, cPlayer & a_Player)
	{
		auto BedPosition = a_Player.GetLastBedPos();

		BLOCKTYPE Type;
		NIBBLETYPE Meta;
		a_ChunkInterface.GetBlockTypeMeta(BedPosition, Type, Meta);

		if (Type != E_BLOCK_BED)
		{
			// Bed was incomplete, just wake:
			a_Player.SetIsInBed(false);
			return;
		}

		if ((Meta & 0x8) == 0)
		{
			// BedPosition is the foot of the bed, adjust to the head:
			BedPosition += MetaDataToDirection(Meta & 0x03);

			a_ChunkInterface.GetBlockTypeMeta(BedPosition, Type, Meta);
			if (Type != E_BLOCK_BED)
			{
				// Bed was incomplete, just wake:
				a_Player.SetIsInBed(false);
				return;
			}
		}

		// Clear the "occupied" bit of the bed's pillow block:
		a_ChunkInterface.SetBlockMeta(BedPosition, Meta & 0x0b);

		// Wake the player:
		a_Player.SetIsInBed(false);
	}

private:

	// Overrides:
	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		const Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override;

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) const override;

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cItem * a_Tool) const override;





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 28;
	}
} ;
