
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../EffectID.h"




class cBlockFenceGateHandler final :
	public cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01>>
{
	using Super = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01>>;

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
				return true;
			default: return false;
		}
	}

	static inline eBlockFace GetFacing(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFenceGate:  return Block::AcaciaFenceGate::Facing(a_Block);
			case BlockType::BirchFenceGate:   return Block::BirchFenceGate::Facing(a_Block);
			case BlockType::CrimsonFenceGate: return Block::CrimsonFenceGate::Facing(a_Block);
			case BlockType::DarkOakFenceGate: return Block::DarkOakFenceGate::Facing(a_Block);
			case BlockType::JungleFenceGate:  return Block::JungleFenceGate::Facing(a_Block);
			case BlockType::OakFenceGate:     return Block::OakFenceGate::Facing(a_Block);
			case BlockType::SpruceFenceGate:  return Block::SpruceFenceGate::Facing(a_Block);
			case BlockType::WarpedFenceGate:  return Block::WarpedFenceGate::Facing(a_Block);
			default: return BLOCK_FACE_NONE;
		}
	}

	static inline bool IsInWall(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFenceGate:  return Block::AcaciaFenceGate::InWall(a_Block);
			case BlockType::BirchFenceGate:   return Block::BirchFenceGate::InWall(a_Block);
			case BlockType::CrimsonFenceGate: return Block::CrimsonFenceGate::InWall(a_Block);
			case BlockType::DarkOakFenceGate: return Block::DarkOakFenceGate::InWall(a_Block);
			case BlockType::JungleFenceGate:  return Block::JungleFenceGate::InWall(a_Block);
			case BlockType::OakFenceGate:     return Block::OakFenceGate::InWall(a_Block);
			case BlockType::SpruceFenceGate:  return Block::SpruceFenceGate::InWall(a_Block);
			case BlockType::WarpedFenceGate:  return Block::WarpedFenceGate::InWall(a_Block);
			default: return false;
		}
	}

	static inline bool IsOpen(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFenceGate:  return Block::AcaciaFenceGate::Open(a_Block);
			case BlockType::BirchFenceGate:   return Block::BirchFenceGate::Open(a_Block);
			case BlockType::CrimsonFenceGate: return Block::CrimsonFenceGate::Open(a_Block);
			case BlockType::DarkOakFenceGate: return Block::DarkOakFenceGate::Open(a_Block);
			case BlockType::JungleFenceGate:  return Block::JungleFenceGate::Open(a_Block);
			case BlockType::OakFenceGate:     return Block::OakFenceGate::Open(a_Block);
			case BlockType::SpruceFenceGate:  return Block::SpruceFenceGate::Open(a_Block);
			case BlockType::WarpedFenceGate:  return Block::WarpedFenceGate::Open(a_Block);
			default: return false;
		}
	}

	static inline bool IsPowered(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFenceGate:  return Block::AcaciaFenceGate::Powered(a_Block);
			case BlockType::BirchFenceGate:   return Block::BirchFenceGate::Powered(a_Block);
			case BlockType::CrimsonFenceGate: return Block::CrimsonFenceGate::Powered(a_Block);
			case BlockType::DarkOakFenceGate: return Block::DarkOakFenceGate::Powered(a_Block);
			case BlockType::JungleFenceGate:  return Block::JungleFenceGate::Powered(a_Block);
			case BlockType::OakFenceGate:     return Block::OakFenceGate::Powered(a_Block);
			case BlockType::SpruceFenceGate:  return Block::SpruceFenceGate::Powered(a_Block);
			case BlockType::WarpedFenceGate:  return Block::WarpedFenceGate::Powered(a_Block);
			default: return false;
		}
	}

// Todo: Add this, once BlockStates are implemented
/*
#define TOGGLE(FenceGateType) \
	a_ChunkInterface.SetBlock(a_Pos, FenceGateType::FenceGateType(FenceGateType::Facing(Self), FenceGateType::InWall(Self), !FenceGateType::Open(Self), !FenceGateType::Powered(Self))); break

	static void Toggle(cChunkInterface & a_ChunkInterface, Vector3i a_Pos)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Pos);
		switch (Self.Type())
		{
			case BlockType::AcaciaFenceGate:  TOGGLE(AcaciaFenceGate);
			case BlockType::BirchFenceGate:   TOGGLE(BirchFenceGate);
			case BlockType::CrimsonFenceGate: TOGGLE(CrimsonFenceGate);
			case BlockType::DarkOakFenceGate: TOGGLE(DarkOakFenceGate);
			case BlockType::JungleFenceGate:  TOGGLE(JungleFenceGate);
			case BlockType::OakFenceGate:     TOGGLE(OakFenceGate);
			case BlockType::SpruceFenceGate:  TOGGLE(SpruceFenceGate);
			case BlockType::WarpedFenceGate:  TOGGLE(WarpedFenceGate);
			default: break;
		}
	}
#undef TOGGLE
*/

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
		NIBBLETYPE OldMetaData = a_ChunkInterface.GetBlockMeta(a_BlockPos);
		NIBBLETYPE NewMetaData = YawToMetaData(a_Player.GetYaw());
		OldMetaData ^= 4;  // Toggle the gate

		if ((OldMetaData & 1) == (NewMetaData & 1))
		{
			// Standing in front of the gate - apply new direction
			a_ChunkInterface.SetBlockMeta(a_BlockPos, (OldMetaData & 4) | (NewMetaData & 3));
		}
		else
		{
			// Standing aside - use last direction
			a_ChunkInterface.SetBlockMeta(a_BlockPos, OldMetaData);
		}
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		switch (m_BlockType)
		{
			case E_BLOCK_OAK_FENCE_GATE: return 13;
			case E_BLOCK_SPRUCE_FENCE_GATE: return 34;
			case E_BLOCK_BIRCH_FENCE_GATE: return 2;
			case E_BLOCK_JUNGLE_FENCE_GATE: return 10;
			case E_BLOCK_DARK_OAK_FENCE_GATE: return 26;
			case E_BLOCK_ACACIA_FENCE_GATE: return 15;
			default:
			{
				ASSERT(!"Unhandled blocktype in fence gate handler!");
				return 0;
			}
		}
	}
} ;




