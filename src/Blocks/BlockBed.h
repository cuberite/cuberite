
// BlockBed.h

#pragma once

#include "BlockEntity.h"
#include "Mixins.h"
#include "ChunkInterface.h"


class cEntity;
class cPlayer;
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

	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override;

	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player,
		const sSetBlock & a_BlockChange
	) const override;





	static void SetBedPos(cPlayer & a_Player, const Vector3i a_BedPosition);





	virtual ColourID GetMapBaseColourID() const override
	{
		return 28;
	}
} ;




