
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../EffectID.h"




class cBlockTrapdoorHandler final :
	public cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x01, 0x02, 0x00, 0x03, false>>
{
	using Super = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x01, 0x02, 0x00, 0x03, false>>;

public:

	using Super::Super;

	static constexpr bool IsBlockTrapdoor(BlockState a_Block)
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
				return true;
			default: return false;
		}
	}


	static constexpr BlockState ToggleTrapdoor(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:  return AcaciaTrapdoor::AcaciaTrapdoor  (AcaciaTrapdoor::Facing(a_Block),  AcaciaTrapdoor::Half(a_Block),  !AcaciaTrapdoor::Open(a_Block),  AcaciaTrapdoor::Powered(a_Block));
			case BlockType::BirchTrapdoor:   return BirchTrapdoor::BirchTrapdoor    (BirchTrapdoor::Facing(a_Block),   BirchTrapdoor::Half(a_Block),   !BirchTrapdoor::Open(a_Block),   BirchTrapdoor::Powered(a_Block));
			case BlockType::CrimsonTrapdoor: return CrimsonTrapdoor::CrimsonTrapdoor(CrimsonTrapdoor::Facing(a_Block), CrimsonTrapdoor::Half(a_Block), !CrimsonTrapdoor::Open(a_Block), CrimsonTrapdoor::Powered(a_Block));
			case BlockType::DarkOakTrapdoor: return DarkOakTrapdoor::DarkOakTrapdoor(DarkOakTrapdoor::Facing(a_Block), DarkOakTrapdoor::Half(a_Block), !DarkOakTrapdoor::Open(a_Block), DarkOakTrapdoor::Powered(a_Block));
			case BlockType::IronTrapdoor:    return IronTrapdoor::IronTrapdoor      (IronTrapdoor::Facing(a_Block),    IronTrapdoor::Half(a_Block),    !IronTrapdoor::Open(a_Block),    IronTrapdoor::Powered(a_Block));
			case BlockType::JungleTrapdoor:  return JungleTrapdoor::JungleTrapdoor  (JungleTrapdoor::Facing(a_Block),  JungleTrapdoor::Half(a_Block),  !JungleTrapdoor::Open(a_Block),  JungleTrapdoor::Powered(a_Block));
			case BlockType::OakTrapdoor:     return OakTrapdoor::OakTrapdoor        (OakTrapdoor::Facing(a_Block),     OakTrapdoor::Half(a_Block),     !OakTrapdoor::Open(a_Block),     OakTrapdoor::Powered(a_Block));
			case BlockType::SpruceTrapdoor:  return SpruceTrapdoor::SpruceTrapdoor  (SpruceTrapdoor::Facing(a_Block),  SpruceTrapdoor::Half(a_Block),  !SpruceTrapdoor::Open(a_Block),  SpruceTrapdoor::Powered(a_Block));
			case BlockType::WarpedTrapdoor:  return WarpedTrapdoor::WarpedTrapdoor  (WarpedTrapdoor::Facing(a_Block),  WarpedTrapdoor::Half(a_Block),  !WarpedTrapdoor::Open(a_Block),  WarpedTrapdoor::Powered(a_Block));
			default: return a_Block;
		}
	}


	static constexpr BlockState ToggleTrapdoorPower(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:  return AcaciaTrapdoor::AcaciaTrapdoor  (AcaciaTrapdoor::Facing(a_Block),  AcaciaTrapdoor::Half(a_Block),  AcaciaTrapdoor::Open(a_Block),  !AcaciaTrapdoor::Powered(a_Block));
			case BlockType::BirchTrapdoor:   return BirchTrapdoor::BirchTrapdoor    (BirchTrapdoor::Facing(a_Block),   BirchTrapdoor::Half(a_Block),   BirchTrapdoor::Open(a_Block),   !BirchTrapdoor::Powered(a_Block));
			case BlockType::CrimsonTrapdoor: return CrimsonTrapdoor::CrimsonTrapdoor(CrimsonTrapdoor::Facing(a_Block), CrimsonTrapdoor::Half(a_Block), CrimsonTrapdoor::Open(a_Block), !CrimsonTrapdoor::Powered(a_Block));
			case BlockType::DarkOakTrapdoor: return DarkOakTrapdoor::DarkOakTrapdoor(DarkOakTrapdoor::Facing(a_Block), DarkOakTrapdoor::Half(a_Block), DarkOakTrapdoor::Open(a_Block), !DarkOakTrapdoor::Powered(a_Block));
			case BlockType::IronTrapdoor:    return IronTrapdoor::IronTrapdoor      (IronTrapdoor::Facing(a_Block),    IronTrapdoor::Half(a_Block),    IronTrapdoor::Open(a_Block),    !IronTrapdoor::Powered(a_Block));
			case BlockType::JungleTrapdoor:  return JungleTrapdoor::JungleTrapdoor  (JungleTrapdoor::Facing(a_Block),  JungleTrapdoor::Half(a_Block),  JungleTrapdoor::Open(a_Block),  !JungleTrapdoor::Powered(a_Block));
			case BlockType::OakTrapdoor:     return OakTrapdoor::OakTrapdoor        (OakTrapdoor::Facing(a_Block),     OakTrapdoor::Half(a_Block),     OakTrapdoor::Open(a_Block),     !OakTrapdoor::Powered(a_Block));
			case BlockType::SpruceTrapdoor:  return SpruceTrapdoor::SpruceTrapdoor  (SpruceTrapdoor::Facing(a_Block),  SpruceTrapdoor::Half(a_Block),  SpruceTrapdoor::Open(a_Block),  !SpruceTrapdoor::Powered(a_Block));
			case BlockType::WarpedTrapdoor:  return WarpedTrapdoor::WarpedTrapdoor  (WarpedTrapdoor::Facing(a_Block),  WarpedTrapdoor::Half(a_Block),  WarpedTrapdoor::Open(a_Block),  !WarpedTrapdoor::Powered(a_Block));
			default: return a_Block;
		}
	}

	static constexpr bool IsTrapdoorOpen(BlockState a_Block)
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
		if (m_BlockType == E_BLOCK_IRON_TRAPDOOR)
		{
			// Iron doors can only be toggled by redstone, not by right-clicking
			return false;
		}

		// Flip the ON bit on / off using the XOR bitwise operation
		NIBBLETYPE Meta = (a_ChunkInterface.GetBlockMeta(a_BlockPos) ^ 0x04);
		a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta);
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





	constexpr static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x3)
		{
			case 0x0: return BLOCK_FACE_ZM;
			case 0x1: return BLOCK_FACE_ZP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_XP;
			default:
			{
				ASSERT(!"Unhandled block meta!");
				return BLOCK_FACE_NONE;
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		switch (m_BlockType)
		{
			case E_BLOCK_TRAPDOOR: return 13;
			case E_BLOCK_IRON_TRAPDOOR: return 6;
			default:
			{
				ASSERT(!"Unhandled blocktype in trapdoor handler!");
				return 0;
			}
		}
	}
};




