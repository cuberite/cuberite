
#pragma once

#include "BlockHandler.h"
#include "Mixins/Mixins.h"
#include "../EffectID.h"




class cBlockTrapdoorHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockTrapdoor(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:
			case BlockType::BirchTrapdoor:
			case BlockType::CrimsonTrapdoor:
			case BlockType::DarkOakTrapdoor:
			case BlockType::IronTrapdoor:
			case BlockType::JungleTrapdoor:
			case BlockType::OakTrapdoor:
			case BlockType::SpruceTrapdoor:
			case BlockType::WarpedTrapdoor:
			case BlockType::BambooTrapdoor:
			case BlockType::CherryTrapdoor:
			case BlockType::MangroveTrapdoor:
			case BlockType::CopperTrapdoor:
			case BlockType::ExposedCopperTrapdoor:
			case BlockType::WeatheredCopperTrapdoor:
			case BlockType::OxidizedCopperTrapdoor:
			case BlockType::WaxedCopperTrapdoor:
			case BlockType::WaxedExposedCopperTrapdoor:
			case BlockType::WaxedWeatheredCopperTrapdoor:
			case BlockType::WaxedOxidizedCopperTrapdoor:
			case BlockType::PaleOakTrapdoor:
				return true;
			default: return false;
		}
	}


#define TOGGLE_TRAPDOOR(TrapdoorType) \
	return TrapdoorType::TrapdoorType  (TrapdoorType::Facing(a_Block),  TrapdoorType::Half(a_Block),  !TrapdoorType::Open(a_Block),  TrapdoorType::Powered(a_Block), TrapdoorType::Waterlogged(a_Block));

	static inline BlockState ToggleTrapdoor(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:  return AcaciaTrapdoor::AcaciaTrapdoor  (AcaciaTrapdoor::Facing(a_Block),  AcaciaTrapdoor::Half(a_Block),  !AcaciaTrapdoor::Open(a_Block),  AcaciaTrapdoor::Powered(a_Block), AcaciaTrapdoor::Waterlogged(a_Block));
			case BlockType::BirchTrapdoor:   return BirchTrapdoor::BirchTrapdoor    (BirchTrapdoor::Facing(a_Block),   BirchTrapdoor::Half(a_Block),   !BirchTrapdoor::Open(a_Block),   BirchTrapdoor::Powered(a_Block), BirchTrapdoor::Waterlogged(a_Block));
			case BlockType::CrimsonTrapdoor: return CrimsonTrapdoor::CrimsonTrapdoor(CrimsonTrapdoor::Facing(a_Block), CrimsonTrapdoor::Half(a_Block), !CrimsonTrapdoor::Open(a_Block), CrimsonTrapdoor::Powered(a_Block), CrimsonTrapdoor::Waterlogged(a_Block));
			case BlockType::DarkOakTrapdoor: return DarkOakTrapdoor::DarkOakTrapdoor(DarkOakTrapdoor::Facing(a_Block), DarkOakTrapdoor::Half(a_Block), !DarkOakTrapdoor::Open(a_Block), DarkOakTrapdoor::Powered(a_Block), DarkOakTrapdoor::Waterlogged(a_Block));
			case BlockType::IronTrapdoor:    return IronTrapdoor::IronTrapdoor      (IronTrapdoor::Facing(a_Block),    IronTrapdoor::Half(a_Block),    !IronTrapdoor::Open(a_Block),    IronTrapdoor::Powered(a_Block), IronTrapdoor::Waterlogged(a_Block));
			case BlockType::JungleTrapdoor:  return JungleTrapdoor::JungleTrapdoor  (JungleTrapdoor::Facing(a_Block),  JungleTrapdoor::Half(a_Block),  !JungleTrapdoor::Open(a_Block),  JungleTrapdoor::Powered(a_Block), JungleTrapdoor::Waterlogged(a_Block));
			case BlockType::OakTrapdoor:     return OakTrapdoor::OakTrapdoor        (OakTrapdoor::Facing(a_Block),     OakTrapdoor::Half(a_Block),     !OakTrapdoor::Open(a_Block),     OakTrapdoor::Powered(a_Block), OakTrapdoor::Waterlogged(a_Block));
			case BlockType::SpruceTrapdoor:  return SpruceTrapdoor::SpruceTrapdoor  (SpruceTrapdoor::Facing(a_Block),  SpruceTrapdoor::Half(a_Block),  !SpruceTrapdoor::Open(a_Block),  SpruceTrapdoor::Powered(a_Block), SpruceTrapdoor::Waterlogged(a_Block));
			case BlockType::WarpedTrapdoor:  return WarpedTrapdoor::WarpedTrapdoor  (WarpedTrapdoor::Facing(a_Block),  WarpedTrapdoor::Half(a_Block),  !WarpedTrapdoor::Open(a_Block),  WarpedTrapdoor::Powered(a_Block), WarpedTrapdoor::Waterlogged(a_Block));
			case BlockType::BambooTrapdoor:               TOGGLE_TRAPDOOR(BambooTrapdoor)
			case BlockType::CherryTrapdoor:               TOGGLE_TRAPDOOR(CherryTrapdoor)
			case BlockType::MangroveTrapdoor:             TOGGLE_TRAPDOOR(MangroveTrapdoor)
			case BlockType::CopperTrapdoor:               TOGGLE_TRAPDOOR(CopperTrapdoor)
			case BlockType::ExposedCopperTrapdoor:        TOGGLE_TRAPDOOR(ExposedCopperTrapdoor)
			case BlockType::WeatheredCopperTrapdoor:      TOGGLE_TRAPDOOR(WeatheredCopperTrapdoor)
			case BlockType::OxidizedCopperTrapdoor:       TOGGLE_TRAPDOOR(OxidizedCopperTrapdoor)
			case BlockType::WaxedCopperTrapdoor:          TOGGLE_TRAPDOOR(WaxedCopperTrapdoor)
			case BlockType::WaxedExposedCopperTrapdoor:   TOGGLE_TRAPDOOR(WaxedExposedCopperTrapdoor)
			case BlockType::WaxedWeatheredCopperTrapdoor: TOGGLE_TRAPDOOR(WaxedWeatheredCopperTrapdoor)
			case BlockType::WaxedOxidizedCopperTrapdoor:  TOGGLE_TRAPDOOR(WaxedOxidizedCopperTrapdoor)
			case BlockType::PaleOakTrapdoor:              TOGGLE_TRAPDOOR(PaleOakTrapdoor)
			default: return a_Block;
		}
	}


#define TOGGLE_TRAPDOOR_POWER(TrapdoorType) \
	return TrapdoorType::TrapdoorType  (TrapdoorType::Facing(a_Block),  TrapdoorType::Half(a_Block),  TrapdoorType::Open(a_Block),  !TrapdoorType::Powered(a_Block), TrapdoorType::Waterlogged(a_Block));


	static inline BlockState ToggleTrapdoorPower(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:  return AcaciaTrapdoor::AcaciaTrapdoor  (AcaciaTrapdoor::Facing(a_Block),  AcaciaTrapdoor::Half(a_Block),  AcaciaTrapdoor::Open(a_Block),  !AcaciaTrapdoor::Powered(a_Block), AcaciaTrapdoor::Waterlogged(a_Block));
			case BlockType::BirchTrapdoor:   return BirchTrapdoor::BirchTrapdoor    (BirchTrapdoor::Facing(a_Block),   BirchTrapdoor::Half(a_Block),   BirchTrapdoor::Open(a_Block),   !BirchTrapdoor::Powered(a_Block), BirchTrapdoor::Waterlogged(a_Block));
			case BlockType::CrimsonTrapdoor: return CrimsonTrapdoor::CrimsonTrapdoor(CrimsonTrapdoor::Facing(a_Block), CrimsonTrapdoor::Half(a_Block), CrimsonTrapdoor::Open(a_Block), !CrimsonTrapdoor::Powered(a_Block), CrimsonTrapdoor::Waterlogged(a_Block));
			case BlockType::DarkOakTrapdoor: return DarkOakTrapdoor::DarkOakTrapdoor(DarkOakTrapdoor::Facing(a_Block), DarkOakTrapdoor::Half(a_Block), DarkOakTrapdoor::Open(a_Block), !DarkOakTrapdoor::Powered(a_Block), DarkOakTrapdoor::Waterlogged(a_Block));
			case BlockType::IronTrapdoor:    return IronTrapdoor::IronTrapdoor      (IronTrapdoor::Facing(a_Block),    IronTrapdoor::Half(a_Block),    IronTrapdoor::Open(a_Block),    !IronTrapdoor::Powered(a_Block), IronTrapdoor::Waterlogged(a_Block));
			case BlockType::JungleTrapdoor:  return JungleTrapdoor::JungleTrapdoor  (JungleTrapdoor::Facing(a_Block),  JungleTrapdoor::Half(a_Block),  JungleTrapdoor::Open(a_Block),  !JungleTrapdoor::Powered(a_Block), JungleTrapdoor::Waterlogged(a_Block));
			case BlockType::OakTrapdoor:     return OakTrapdoor::OakTrapdoor        (OakTrapdoor::Facing(a_Block),     OakTrapdoor::Half(a_Block),     OakTrapdoor::Open(a_Block),     !OakTrapdoor::Powered(a_Block), OakTrapdoor::Waterlogged(a_Block));
			case BlockType::SpruceTrapdoor:  return SpruceTrapdoor::SpruceTrapdoor  (SpruceTrapdoor::Facing(a_Block),  SpruceTrapdoor::Half(a_Block),  SpruceTrapdoor::Open(a_Block),  !SpruceTrapdoor::Powered(a_Block), SpruceTrapdoor::Waterlogged(a_Block));
			case BlockType::WarpedTrapdoor:  return WarpedTrapdoor::WarpedTrapdoor  (WarpedTrapdoor::Facing(a_Block),  WarpedTrapdoor::Half(a_Block),  WarpedTrapdoor::Open(a_Block),  !WarpedTrapdoor::Powered(a_Block), WarpedTrapdoor::Waterlogged(a_Block));
			case BlockType::BambooTrapdoor:               TOGGLE_TRAPDOOR_POWER(BambooTrapdoor)
			case BlockType::CherryTrapdoor:               TOGGLE_TRAPDOOR_POWER(CherryTrapdoor)
			case BlockType::MangroveTrapdoor:             TOGGLE_TRAPDOOR_POWER(MangroveTrapdoor)
			case BlockType::CopperTrapdoor:               TOGGLE_TRAPDOOR_POWER(CopperTrapdoor)
			case BlockType::ExposedCopperTrapdoor:        TOGGLE_TRAPDOOR_POWER(ExposedCopperTrapdoor)
			case BlockType::WeatheredCopperTrapdoor:      TOGGLE_TRAPDOOR_POWER(WeatheredCopperTrapdoor)
			case BlockType::OxidizedCopperTrapdoor:       TOGGLE_TRAPDOOR_POWER(OxidizedCopperTrapdoor)
			case BlockType::WaxedCopperTrapdoor:          TOGGLE_TRAPDOOR_POWER(WaxedCopperTrapdoor)
			case BlockType::WaxedExposedCopperTrapdoor:   TOGGLE_TRAPDOOR_POWER(WaxedExposedCopperTrapdoor)
			case BlockType::WaxedWeatheredCopperTrapdoor: TOGGLE_TRAPDOOR_POWER(WaxedWeatheredCopperTrapdoor)
			case BlockType::WaxedOxidizedCopperTrapdoor:  TOGGLE_TRAPDOOR_POWER(WaxedOxidizedCopperTrapdoor)
			case BlockType::PaleOakTrapdoor:              TOGGLE_TRAPDOOR_POWER(PaleOakTrapdoor)
			default: return a_Block;
		}
	}

#define IS_TRAPDOOR_OPEN(TrapdoorType) \
	return TrapdoorType::Open(a_Block);

	static inline bool IsTrapdoorOpen(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:  return AcaciaTrapdoor::Open(a_Block);
			case BlockType::BirchTrapdoor:   return BirchTrapdoor::Open(a_Block);
			case BlockType::CrimsonTrapdoor: return CrimsonTrapdoor::Open(a_Block);
			case BlockType::DarkOakTrapdoor: return DarkOakTrapdoor::Open(a_Block);
			case BlockType::IronTrapdoor:    return IronTrapdoor::Open(a_Block);
			case BlockType::JungleTrapdoor:  return JungleTrapdoor::Open(a_Block);
			case BlockType::OakTrapdoor:     return OakTrapdoor::Open(a_Block);
			case BlockType::SpruceTrapdoor:  return SpruceTrapdoor::Open(a_Block);
			case BlockType::WarpedTrapdoor:  return WarpedTrapdoor::Open(a_Block);
			case BlockType::BambooTrapdoor:               IS_TRAPDOOR_OPEN(BambooTrapdoor)
			case BlockType::CherryTrapdoor:               IS_TRAPDOOR_OPEN(CherryTrapdoor)
			case BlockType::MangroveTrapdoor:             IS_TRAPDOOR_OPEN(MangroveTrapdoor)
			case BlockType::CopperTrapdoor:               IS_TRAPDOOR_OPEN(CopperTrapdoor)
			case BlockType::ExposedCopperTrapdoor:        IS_TRAPDOOR_OPEN(ExposedCopperTrapdoor)
			case BlockType::WeatheredCopperTrapdoor:      IS_TRAPDOOR_OPEN(WeatheredCopperTrapdoor)
			case BlockType::OxidizedCopperTrapdoor:       IS_TRAPDOOR_OPEN(OxidizedCopperTrapdoor)
			case BlockType::WaxedCopperTrapdoor:          IS_TRAPDOOR_OPEN(WaxedCopperTrapdoor)
			case BlockType::WaxedExposedCopperTrapdoor:   IS_TRAPDOOR_OPEN(WaxedExposedCopperTrapdoor)
			case BlockType::WaxedWeatheredCopperTrapdoor: IS_TRAPDOOR_OPEN(WaxedWeatheredCopperTrapdoor)
			case BlockType::WaxedOxidizedCopperTrapdoor:  IS_TRAPDOOR_OPEN(WaxedOxidizedCopperTrapdoor)
			case BlockType::PaleOakTrapdoor:              IS_TRAPDOOR_OPEN(PaleOakTrapdoor)
			default: return false;
		}
	}

private:

	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		if (m_BlockType == BlockType::IronTrapdoor)
		{
			// Iron doors can only be toggled by redstone, not by right-clicking
			return false;
		}

		// Flip the ON bit on / off using the XOR bitwise operation
		auto Self = a_ChunkInterface.GetBlock(a_BlockPos);
		a_ChunkInterface.SetBlock(a_BlockPos, ToggleTrapdoor(Self));
		a_WorldInterface.GetBroadcastManager().BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_FENCE_GATE_OPEN, a_BlockPos, 0, a_Player.GetClientHandle());

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
		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaTrapdoor:  return 15;
			case BlockType::BirchTrapdoor:   return 2;
			case BlockType::CrimsonTrapdoor: return 35;
			case BlockType::DarkOakTrapdoor: return 26;
			case BlockType::IronTrapdoor:    return 6;
			case BlockType::JungleTrapdoor:  return 10;
			case BlockType::OakTrapdoor:     return 13;
			case BlockType::SpruceTrapdoor:  return 26;
			case BlockType::WarpedTrapdoor:  return 56;
			case BlockType::BambooTrapdoor:
			case BlockType::CherryTrapdoor:
			case BlockType::MangroveTrapdoor:
			case BlockType::CopperTrapdoor:
			case BlockType::ExposedCopperTrapdoor:
			case BlockType::WeatheredCopperTrapdoor:
			case BlockType::OxidizedCopperTrapdoor:
			case BlockType::WaxedCopperTrapdoor:
			case BlockType::WaxedExposedCopperTrapdoor:
			case BlockType::WaxedWeatheredCopperTrapdoor:
			case BlockType::WaxedOxidizedCopperTrapdoor:
			case BlockType::PaleOakTrapdoor: return 0;
			default:
			{
				ASSERT(!"Unhandled blocktype in trapdoor handler!");
				return 0;
			}
		}
	}
};




