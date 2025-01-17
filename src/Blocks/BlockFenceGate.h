
#pragma once

#include "BlockHandler.h"
#include "Mixins/Mixins.h"
#include "../EffectID.h"




class cBlockFenceGateHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsFenceGate(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFenceGate:
			case BlockType::BirchFenceGate:
			case BlockType::CrimsonFenceGate:
			case BlockType::DarkOakFenceGate:
			case BlockType::JungleFenceGate:
			case BlockType::OakFenceGate:
			case BlockType::SpruceFenceGate:
			case BlockType::WarpedFenceGate:
			case BlockType::BambooFenceGate:
			case BlockType::CherryFenceGate:
			case BlockType::MangroveFenceGate:
				return true;
			default: return false;
		}
	}



#define TOGGLE(FenceGateType) \
	a_ChunkInterface.SetBlock(a_Pos, FenceGateType::FenceGateType(FenceGateType::Facing(Self), FenceGateType::InWall(Self), !FenceGateType::Open(Self), !FenceGateType::Powered(Self))); break

	static void Toggle(cChunkInterface & a_ChunkInterface, Vector3i a_Pos)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Pos);
		switch (Self.Type())
		{
			case BlockType::AcaciaFenceGate:   TOGGLE(AcaciaFenceGate);
			case BlockType::BirchFenceGate:    TOGGLE(BirchFenceGate);
			case BlockType::CrimsonFenceGate:  TOGGLE(CrimsonFenceGate);
			case BlockType::DarkOakFenceGate:  TOGGLE(DarkOakFenceGate);
			case BlockType::JungleFenceGate:   TOGGLE(JungleFenceGate);
			case BlockType::OakFenceGate:      TOGGLE(OakFenceGate);
			case BlockType::SpruceFenceGate:   TOGGLE(SpruceFenceGate);
			case BlockType::WarpedFenceGate:   TOGGLE(WarpedFenceGate);
			case BlockType::BambooFenceGate:   TOGGLE(BambooFenceGate);
			case BlockType::CherryFenceGate:   TOGGLE(CherryFenceGate);
			case BlockType::MangroveFenceGate: TOGGLE(MangroveFenceGate);
			default: break;
		}
	}

private:

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{

		Toggle(a_ChunkInterface, a_BlockPos);
		a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_FENCE_GATE_OPEN, a_BlockPos, 0, a_Player.GetClientHandle());
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
		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaFenceGate:  return 15;
			case BlockType::BirchFenceGate:   return 2;
			case BlockType::CrimsonFenceGate: return 35;
			case BlockType::DarkOakFenceGate: return 26;
			case BlockType::JungleFenceGate:  return 10;
			case BlockType::OakFenceGate:     return 13;
			case BlockType::SpruceFenceGate:  return 34;
			case BlockType::WarpedFenceGate:  return 56;
			case BlockType::BambooFenceGate:
			case BlockType::CherryFenceGate:
			case BlockType::MangroveFenceGate:
			case BlockType::PaleOakFence:     return 0;
			default:
			{
				ASSERT(!"Unhandled blocktype in fence gate handler!");
				return 0;
			}
		}
	}
} ;




