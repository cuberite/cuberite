
#include "Globals.h"
#include "BlockDoor.h"
#include "../EffectID.h"





void cBlockDoorHandler::OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_OldBlock,
		const cEntity * a_Digger
) const
{
	UNUSED(a_Digger);
	// A part of the multiblock door was broken; the relevant half will drop any pickups as required.
	// All that is left to do is to delete the other half of the multiblock.

	if (IsTop(a_OldBlock))
	{
		const auto Lower = a_BlockPos.addedY(-1);
		if ((Lower.y >= 0) && IsBlockDoor(a_ChunkInterface.GetBlock(Lower)))
		{
			// Was upper part of door, remove lower:
			a_ChunkInterface.SetBlock(Lower, Block::Air::Air());
		}
	}
	else
	{
		const auto Upper = a_BlockPos.addedY(1);
		if ((Upper.y < cChunkDef::Height) && IsBlockDoor(a_ChunkInterface.GetBlock(Upper)))
		{
			// Was lower part, remove upper:
			a_ChunkInterface.SetBlock(Upper, Block::Air::Air());
		}
	}
}





bool cBlockDoorHandler::OnUse(
	cChunkInterface & a_ChunkInterface,
	cWorldInterface & a_WorldInterface,
	cPlayer & a_Player,
	const Vector3i a_BlockPos,
	eBlockFace a_BlockFace,
	const Vector3i a_CursorPos
) const
{
	UNUSED(a_WorldInterface);
	UNUSED(a_BlockFace);
	UNUSED(a_CursorPos);

	switch (a_ChunkInterface.GetBlock(a_BlockPos).Type())
	{
		default:
		{
			ASSERT(!"Unhandled door block type");
		}
		case BlockType::AcaciaDoor:
		case BlockType::BirchDoor:
		case BlockType::CrimsonDoor:
		case BlockType::DarkOakDoor:
		case BlockType::JungleDoor:
		case BlockType::SpruceDoor:
		case BlockType::OakDoor:
		case BlockType::WarpedDoor:
		case BlockType::BambooDoor:
		case BlockType::CherryDoor:
		case BlockType::CopperDoor:
		case BlockType::MangroveDoor:
		case BlockType::ExposedCopperDoor:
		case BlockType::WeatheredCopperDoor:
		case BlockType::OxidizedCopperDoor:
		case BlockType::WaxedCopperDoor:
		case BlockType::WaxedExposedCopperDoor:
		case BlockType::WaxedWeatheredCopperDoor:
		case BlockType::WaxedOxidizedCopperDoor:
		case BlockType::PaleOakDoor:
		{
			ChangeDoor(a_ChunkInterface, a_BlockPos);
			a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, a_BlockPos, 0, a_Player.GetClientHandle());
			break;
		}
		case BlockType::IronDoor:
		{
			// Prevent iron door from opening on player click (#2415):
			OnCancelRightClick(a_ChunkInterface, a_WorldInterface, a_Player, a_BlockPos, a_BlockFace);

			// Allow placement actions to instead take place:
			return false;
		}
	}

	return true;
}





void cBlockDoorHandler::OnCancelRightClick(
	cChunkInterface & a_ChunkInterface,
	cWorldInterface & a_WorldInterface,
	cPlayer & a_Player,
	const Vector3i a_BlockPos,
	eBlockFace a_BlockFace
) const
{
	UNUSED(a_ChunkInterface);
	UNUSED(a_BlockFace);

	a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	auto Self = a_ChunkInterface.GetBlock(a_BlockPos);

	if (IsTop(Self))
	{
		// Current block is top of the door, send the bottom part:
		a_WorldInterface.SendBlockTo(a_BlockPos.addedY(-1), a_Player);
	}
	else
	{
		// Current block is bottom of the door, send the top part:
		a_WorldInterface.SendBlockTo(a_BlockPos.addedY(1), a_Player);
	}
}





cBoundingBox cBlockDoorHandler::GetPlacementCollisionBox(BlockState a_XM, BlockState a_XP, BlockState a_YM, BlockState a_YP, BlockState a_ZM, BlockState a_ZP) const
{
	// Doors can be placed inside the player
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





BlockState cBlockDoorHandler::RotateCCW(BlockState a_Block) const
{
#define ROTATE_CCW(DoorType) \
	return DoorType::DoorType(RotateBlockFaceCCW(DoorType::Facing(a_Block)), DoorType::Half(a_Block), DoorType::Hinge(a_Block), DoorType::Open(a_Block), DoorType::Powered(a_Block));
	using namespace Block;
	switch (a_Block.Type())
	{
		case BlockType::AcaciaDoor:  return AcaciaDoor ::AcaciaDoor (RotateBlockFaceCCW(AcaciaDoor:: Facing(a_Block)), AcaciaDoor:: Half(a_Block), AcaciaDoor:: Hinge(a_Block), AcaciaDoor:: Open(a_Block), AcaciaDoor:: Powered(a_Block));
		case BlockType::BirchDoor:   return BirchDoor  ::BirchDoor  (RotateBlockFaceCCW(BirchDoor::  Facing(a_Block)), BirchDoor::  Half(a_Block), BirchDoor::  Hinge(a_Block), BirchDoor::  Open(a_Block), BirchDoor::  Powered(a_Block));
		case BlockType::CrimsonDoor: return CrimsonDoor::CrimsonDoor(RotateBlockFaceCCW(CrimsonDoor::Facing(a_Block)), CrimsonDoor::Half(a_Block), CrimsonDoor::Hinge(a_Block), CrimsonDoor::Open(a_Block), CrimsonDoor::Powered(a_Block));
		case BlockType::DarkOakDoor: return DarkOakDoor::DarkOakDoor(RotateBlockFaceCCW(DarkOakDoor::Facing(a_Block)), DarkOakDoor::Half(a_Block), DarkOakDoor::Hinge(a_Block), DarkOakDoor::Open(a_Block), DarkOakDoor::Powered(a_Block));
		case BlockType::JungleDoor:  return JungleDoor::JungleDoor  (RotateBlockFaceCCW(JungleDoor:: Facing(a_Block)), JungleDoor:: Half(a_Block), JungleDoor:: Hinge(a_Block), JungleDoor:: Open(a_Block), JungleDoor:: Powered(a_Block));
		case BlockType::OakDoor:     return OakDoor::   OakDoor     (RotateBlockFaceCCW(OakDoor::    Facing(a_Block)), OakDoor::    Half(a_Block), OakDoor::    Hinge(a_Block), OakDoor::    Open(a_Block), OakDoor::    Powered(a_Block));
		case BlockType::SpruceDoor:  return SpruceDoor::SpruceDoor  (RotateBlockFaceCCW(SpruceDoor:: Facing(a_Block)), SpruceDoor:: Half(a_Block), SpruceDoor:: Hinge(a_Block), SpruceDoor:: Open(a_Block), SpruceDoor:: Powered(a_Block));
		case BlockType::WarpedDoor:  return WarpedDoor::WarpedDoor  (RotateBlockFaceCCW(WarpedDoor:: Facing(a_Block)), WarpedDoor:: Half(a_Block), WarpedDoor:: Hinge(a_Block), WarpedDoor:: Open(a_Block), WarpedDoor:: Powered(a_Block));
		case BlockType::IronDoor:    return IronDoor::  IronDoor    (RotateBlockFaceCCW(IronDoor::   Facing(a_Block)), IronDoor::   Half(a_Block), IronDoor::   Hinge(a_Block), IronDoor::   Open(a_Block), IronDoor::   Powered(a_Block));
		case BlockType::BambooDoor:               ROTATE_CCW(BambooDoor)
		case BlockType::CherryDoor:               ROTATE_CCW(CherryDoor)
		case BlockType::CopperDoor:               ROTATE_CCW(CopperDoor)
		case BlockType::MangroveDoor:             ROTATE_CCW(MangroveDoor)
		case BlockType::ExposedCopperDoor:        ROTATE_CCW(ExposedCopperDoor)
		case BlockType::WeatheredCopperDoor:      ROTATE_CCW(WeatheredCopperDoor)
		case BlockType::OxidizedCopperDoor:       ROTATE_CCW(OxidizedCopperDoor)
		case BlockType::WaxedCopperDoor:          ROTATE_CCW(WaxedCopperDoor)
		case BlockType::WaxedExposedCopperDoor:   ROTATE_CCW(WaxedExposedCopperDoor)
		case BlockType::WaxedWeatheredCopperDoor: ROTATE_CCW(WaxedWeatheredCopperDoor)
		case BlockType::WaxedOxidizedCopperDoor:  ROTATE_CCW(WaxedOxidizedCopperDoor)
		case BlockType::PaleOakDoor:              ROTATE_CCW(PaleOakDoor)
		default: return a_Block;
	}
}





BlockState cBlockDoorHandler::RotateCW(BlockState a_Block) const
{
#define ROTATE_CW(DoorType) \
	return DoorType::DoorType(RotateBlockFaceCW(DoorType::Facing(a_Block)), DoorType::Half(a_Block), DoorType::Hinge(a_Block), DoorType::Open(a_Block), DoorType::Powered(a_Block));
	using namespace Block;
	switch (a_Block.Type())
	{
		case BlockType::AcaciaDoor:  return AcaciaDoor ::AcaciaDoor (RotateBlockFaceCW(AcaciaDoor:: Facing(a_Block)), AcaciaDoor:: Half(a_Block), AcaciaDoor:: Hinge(a_Block), AcaciaDoor:: Open(a_Block), AcaciaDoor:: Powered(a_Block));
		case BlockType::BirchDoor:   return BirchDoor  ::BirchDoor  (RotateBlockFaceCW(BirchDoor::  Facing(a_Block)), BirchDoor::  Half(a_Block), BirchDoor::  Hinge(a_Block), BirchDoor::  Open(a_Block), BirchDoor::  Powered(a_Block));
		case BlockType::CrimsonDoor: return CrimsonDoor::CrimsonDoor(RotateBlockFaceCW(CrimsonDoor::Facing(a_Block)), CrimsonDoor::Half(a_Block), CrimsonDoor::Hinge(a_Block), CrimsonDoor::Open(a_Block), CrimsonDoor::Powered(a_Block));
		case BlockType::DarkOakDoor: return DarkOakDoor::DarkOakDoor(RotateBlockFaceCW(DarkOakDoor::Facing(a_Block)), DarkOakDoor::Half(a_Block), DarkOakDoor::Hinge(a_Block), DarkOakDoor::Open(a_Block), DarkOakDoor::Powered(a_Block));
		case BlockType::JungleDoor:  return JungleDoor::JungleDoor  (RotateBlockFaceCW(JungleDoor:: Facing(a_Block)), JungleDoor:: Half(a_Block), JungleDoor:: Hinge(a_Block), JungleDoor:: Open(a_Block), JungleDoor:: Powered(a_Block));
		case BlockType::OakDoor:     return OakDoor::   OakDoor     (RotateBlockFaceCW(OakDoor::    Facing(a_Block)), OakDoor::    Half(a_Block), OakDoor::    Hinge(a_Block), OakDoor::    Open(a_Block), OakDoor::    Powered(a_Block));
		case BlockType::SpruceDoor:  return SpruceDoor::SpruceDoor  (RotateBlockFaceCW(SpruceDoor:: Facing(a_Block)), SpruceDoor:: Half(a_Block), SpruceDoor:: Hinge(a_Block), SpruceDoor:: Open(a_Block), SpruceDoor:: Powered(a_Block));
		case BlockType::WarpedDoor:  return WarpedDoor::WarpedDoor  (RotateBlockFaceCW(WarpedDoor:: Facing(a_Block)), WarpedDoor:: Half(a_Block), WarpedDoor:: Hinge(a_Block), WarpedDoor:: Open(a_Block), WarpedDoor:: Powered(a_Block));
		case BlockType::IronDoor:    return IronDoor::  IronDoor    (RotateBlockFaceCW(IronDoor::   Facing(a_Block)), IronDoor::   Half(a_Block), IronDoor::   Hinge(a_Block), IronDoor::   Open(a_Block), IronDoor::   Powered(a_Block));
		case BlockType::BambooDoor:               ROTATE_CW(BambooDoor)
		case BlockType::CherryDoor:               ROTATE_CW(CherryDoor)
		case BlockType::CopperDoor:               ROTATE_CW(CopperDoor)
		case BlockType::MangroveDoor:             ROTATE_CW(MangroveDoor)
		case BlockType::ExposedCopperDoor:        ROTATE_CW(ExposedCopperDoor)
		case BlockType::WeatheredCopperDoor:      ROTATE_CW(WeatheredCopperDoor)
		case BlockType::OxidizedCopperDoor:       ROTATE_CW(OxidizedCopperDoor)
		case BlockType::WaxedCopperDoor:          ROTATE_CW(WaxedCopperDoor)
		case BlockType::WaxedExposedCopperDoor:   ROTATE_CW(WaxedExposedCopperDoor)
		case BlockType::WaxedWeatheredCopperDoor: ROTATE_CW(WaxedWeatheredCopperDoor)
		case BlockType::WaxedOxidizedCopperDoor:  ROTATE_CW(WaxedOxidizedCopperDoor)
		case BlockType::PaleOakDoor:              ROTATE_CW(PaleOakDoor)
		default: return a_Block;
	}
}





BlockState cBlockDoorHandler::MirrorXY(BlockState a_Block) const
{
#define MIRROR_XY(DoorType) \
	return DoorType::DoorType(MirrorBlockFaceXY(DoorType::Facing(a_Block)), DoorType::Half(a_Block), DoorType::Hinge(a_Block), DoorType::Open(a_Block), DoorType::Powered(a_Block));
	using namespace Block;
	switch (a_Block.Type())
	{
		case BlockType::AcaciaDoor:  return AcaciaDoor ::AcaciaDoor (MirrorBlockFaceXY(AcaciaDoor:: Facing(a_Block)), AcaciaDoor:: Half(a_Block), AcaciaDoor:: Hinge(a_Block), AcaciaDoor:: Open(a_Block), AcaciaDoor:: Powered(a_Block));
		case BlockType::BirchDoor:   return BirchDoor  ::BirchDoor  (MirrorBlockFaceXY(BirchDoor::  Facing(a_Block)), BirchDoor::  Half(a_Block), BirchDoor::  Hinge(a_Block), BirchDoor::  Open(a_Block), BirchDoor::  Powered(a_Block));
		case BlockType::CrimsonDoor: return CrimsonDoor::CrimsonDoor(MirrorBlockFaceXY(CrimsonDoor::Facing(a_Block)), CrimsonDoor::Half(a_Block), CrimsonDoor::Hinge(a_Block), CrimsonDoor::Open(a_Block), CrimsonDoor::Powered(a_Block));
		case BlockType::DarkOakDoor: return DarkOakDoor::DarkOakDoor(MirrorBlockFaceXY(DarkOakDoor::Facing(a_Block)), DarkOakDoor::Half(a_Block), DarkOakDoor::Hinge(a_Block), DarkOakDoor::Open(a_Block), DarkOakDoor::Powered(a_Block));
		case BlockType::JungleDoor:  return JungleDoor::JungleDoor  (MirrorBlockFaceXY(JungleDoor:: Facing(a_Block)), JungleDoor:: Half(a_Block), JungleDoor:: Hinge(a_Block), JungleDoor:: Open(a_Block), JungleDoor:: Powered(a_Block));
		case BlockType::OakDoor:     return OakDoor::   OakDoor     (MirrorBlockFaceXY(OakDoor::    Facing(a_Block)), OakDoor::    Half(a_Block), OakDoor::    Hinge(a_Block), OakDoor::    Open(a_Block), OakDoor::    Powered(a_Block));
		case BlockType::SpruceDoor:  return SpruceDoor::SpruceDoor  (MirrorBlockFaceXY(SpruceDoor:: Facing(a_Block)), SpruceDoor:: Half(a_Block), SpruceDoor:: Hinge(a_Block), SpruceDoor:: Open(a_Block), SpruceDoor:: Powered(a_Block));
		case BlockType::WarpedDoor:  return WarpedDoor::WarpedDoor  (MirrorBlockFaceXY(WarpedDoor:: Facing(a_Block)), WarpedDoor:: Half(a_Block), WarpedDoor:: Hinge(a_Block), WarpedDoor:: Open(a_Block), WarpedDoor:: Powered(a_Block));
		case BlockType::IronDoor:    return IronDoor::  IronDoor    (MirrorBlockFaceXY(IronDoor::   Facing(a_Block)), IronDoor::   Half(a_Block), IronDoor::   Hinge(a_Block), IronDoor::   Open(a_Block), IronDoor::   Powered(a_Block));
		case BlockType::BambooDoor:               MIRROR_XY(BambooDoor)
		case BlockType::CherryDoor:               MIRROR_XY(CherryDoor)
		case BlockType::CopperDoor:               MIRROR_XY(CopperDoor)
		case BlockType::MangroveDoor:             MIRROR_XY(MangroveDoor)
		case BlockType::ExposedCopperDoor:        MIRROR_XY(ExposedCopperDoor)
		case BlockType::WeatheredCopperDoor:      MIRROR_XY(WeatheredCopperDoor)
		case BlockType::OxidizedCopperDoor:       MIRROR_XY(OxidizedCopperDoor)
		case BlockType::WaxedCopperDoor:          MIRROR_XY(WaxedCopperDoor)
		case BlockType::WaxedExposedCopperDoor:   MIRROR_XY(WaxedExposedCopperDoor)
		case BlockType::WaxedWeatheredCopperDoor: MIRROR_XY(WaxedWeatheredCopperDoor)
		case BlockType::WaxedOxidizedCopperDoor:  MIRROR_XY(WaxedOxidizedCopperDoor)
		case BlockType::PaleOakDoor:              MIRROR_XY(PaleOakDoor)
		default: return a_Block;
	}
}





BlockState cBlockDoorHandler::MirrorYZ(BlockState a_Block) const
{
#define MIRROR_YZ(DoorType) \
	return DoorType::DoorType(MirrorBlockFaceYZ(DoorType::Facing(a_Block)), DoorType::Half(a_Block), DoorType::Hinge(a_Block), DoorType::Open(a_Block), DoorType::Powered(a_Block));
	using namespace Block;
	switch (a_Block.Type())
	{
		case BlockType::AcaciaDoor:  return AcaciaDoor ::AcaciaDoor (MirrorBlockFaceYZ(AcaciaDoor:: Facing(a_Block)), AcaciaDoor:: Half(a_Block), AcaciaDoor:: Hinge(a_Block), AcaciaDoor:: Open(a_Block), AcaciaDoor:: Powered(a_Block));
		case BlockType::BirchDoor:   return BirchDoor  ::BirchDoor  (MirrorBlockFaceYZ(BirchDoor::  Facing(a_Block)), BirchDoor::  Half(a_Block), BirchDoor::  Hinge(a_Block), BirchDoor::  Open(a_Block), BirchDoor::  Powered(a_Block));
		case BlockType::CrimsonDoor: return CrimsonDoor::CrimsonDoor(MirrorBlockFaceYZ(CrimsonDoor::Facing(a_Block)), CrimsonDoor::Half(a_Block), CrimsonDoor::Hinge(a_Block), CrimsonDoor::Open(a_Block), CrimsonDoor::Powered(a_Block));
		case BlockType::DarkOakDoor: return DarkOakDoor::DarkOakDoor(MirrorBlockFaceYZ(DarkOakDoor::Facing(a_Block)), DarkOakDoor::Half(a_Block), DarkOakDoor::Hinge(a_Block), DarkOakDoor::Open(a_Block), DarkOakDoor::Powered(a_Block));
		case BlockType::JungleDoor:  return JungleDoor::JungleDoor  (MirrorBlockFaceYZ(JungleDoor:: Facing(a_Block)), JungleDoor:: Half(a_Block), JungleDoor:: Hinge(a_Block), JungleDoor:: Open(a_Block), JungleDoor:: Powered(a_Block));
		case BlockType::OakDoor:     return OakDoor::   OakDoor     (MirrorBlockFaceYZ(OakDoor::    Facing(a_Block)), OakDoor::    Half(a_Block), OakDoor::    Hinge(a_Block), OakDoor::    Open(a_Block), OakDoor::    Powered(a_Block));
		case BlockType::SpruceDoor:  return SpruceDoor::SpruceDoor  (MirrorBlockFaceYZ(SpruceDoor:: Facing(a_Block)), SpruceDoor:: Half(a_Block), SpruceDoor:: Hinge(a_Block), SpruceDoor:: Open(a_Block), SpruceDoor:: Powered(a_Block));
		case BlockType::WarpedDoor:  return WarpedDoor::WarpedDoor  (MirrorBlockFaceYZ(WarpedDoor:: Facing(a_Block)), WarpedDoor:: Half(a_Block), WarpedDoor:: Hinge(a_Block), WarpedDoor:: Open(a_Block), WarpedDoor:: Powered(a_Block));
		case BlockType::IronDoor:    return IronDoor::  IronDoor    (MirrorBlockFaceYZ(IronDoor::   Facing(a_Block)), IronDoor::   Half(a_Block), IronDoor::   Hinge(a_Block), IronDoor::   Open(a_Block), IronDoor::   Powered(a_Block));
		case BlockType::BambooDoor:               MIRROR_YZ(BambooDoor)
		case BlockType::CherryDoor:               MIRROR_YZ(CherryDoor)
		case BlockType::CopperDoor:               MIRROR_YZ(CopperDoor)
		case BlockType::MangroveDoor:             MIRROR_YZ(MangroveDoor)
		case BlockType::ExposedCopperDoor:        MIRROR_YZ(ExposedCopperDoor)
		case BlockType::WeatheredCopperDoor:      MIRROR_YZ(WeatheredCopperDoor)
		case BlockType::OxidizedCopperDoor:       MIRROR_YZ(OxidizedCopperDoor)
		case BlockType::WaxedCopperDoor:          MIRROR_YZ(WaxedCopperDoor)
		case BlockType::WaxedExposedCopperDoor:   MIRROR_YZ(WaxedExposedCopperDoor)
		case BlockType::WaxedWeatheredCopperDoor: MIRROR_YZ(WaxedWeatheredCopperDoor)
		case BlockType::WaxedOxidizedCopperDoor:  MIRROR_YZ(WaxedOxidizedCopperDoor)
		case BlockType::PaleOakDoor:              MIRROR_YZ(PaleOakDoor)
		default: return a_Block;
	}
}



