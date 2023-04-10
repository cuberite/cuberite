
#pragma once

#include "BlockHandler.h"
#include "../BoundingBox.h"
#include "../EffectID.h"
#include "../Entities/LeashKnot.h"
#include "../BoundingBox.h"
#include "../Mobs/PassiveMonster.h"



class cBlockFenceHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;


	static inline bool IsBlockFence(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFence:
			case BlockType::BirchFence:
			case BlockType::DarkOakFence:
			case BlockType::JungleFence:
			case BlockType::NetherBrickFence:
			case BlockType::OakFence:
			case BlockType::SpruceFence:
			case BlockType::WarpedFence:
				return true;
			default: return false;
		}
	}

	static inline bool IsConnectedNorth(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFence:      return Block::AcaciaFence::North(a_Block);
			case BlockType::BirchFence:       return Block::BirchFence::North(a_Block);
			case BlockType::DarkOakFence:     return Block::DarkOakFence::North(a_Block);
			case BlockType::JungleFence:      return Block::JungleFence::North(a_Block);
			case BlockType::NetherBrickFence: return Block::NetherBrickFence::North(a_Block);
			case BlockType::OakFence:         return Block::OakFence::North(a_Block);
			case BlockType::SpruceFence:      return Block::SpruceFence::North(a_Block);
			case BlockType::WarpedFence:      return Block::WarpedFence::North(a_Block);
			default: return false;
		}
	}

	static inline bool IsConnectedEast(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFence:      return Block::AcaciaFence::East(a_Block);
			case BlockType::BirchFence:       return Block::BirchFence::East(a_Block);
			case BlockType::DarkOakFence:     return Block::DarkOakFence::East(a_Block);
			case BlockType::JungleFence:      return Block::JungleFence::East(a_Block);
			case BlockType::NetherBrickFence: return Block::NetherBrickFence::East(a_Block);
			case BlockType::OakFence:         return Block::OakFence::East(a_Block);
			case BlockType::SpruceFence:      return Block::SpruceFence::East(a_Block);
			case BlockType::WarpedFence:      return Block::WarpedFence::East(a_Block);
			default: return false;
		}
	}

	static inline bool IsConnectedSouth(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFence:      return Block::AcaciaFence::South(a_Block);
			case BlockType::BirchFence:       return Block::BirchFence::South(a_Block);
			case BlockType::DarkOakFence:     return Block::DarkOakFence::South(a_Block);
			case BlockType::JungleFence:      return Block::JungleFence::South(a_Block);
			case BlockType::NetherBrickFence: return Block::NetherBrickFence::South(a_Block);
			case BlockType::OakFence:         return Block::OakFence::South(a_Block);
			case BlockType::SpruceFence:      return Block::SpruceFence::South(a_Block);
			case BlockType::WarpedFence:      return Block::WarpedFence::South(a_Block);
			default: return false;
		}
	}

	static inline bool IsConnectedWest(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaFence:      return Block::AcaciaFence::West(a_Block);
			case BlockType::BirchFence:       return Block::BirchFence::West(a_Block);
			case BlockType::DarkOakFence:     return Block::DarkOakFence::West(a_Block);
			case BlockType::JungleFence:      return Block::JungleFence::West(a_Block);
			case BlockType::NetherBrickFence: return Block::NetherBrickFence::West(a_Block);
			case BlockType::OakFence:         return Block::OakFence::West(a_Block);
			case BlockType::SpruceFence:      return Block::SpruceFence::West(a_Block);
			case BlockType::WarpedFence:      return Block::WarpedFence::West(a_Block);
			default: return false;
		}
	}

private:

	// These are the min and max coordinates (X and Z) for a straight fence.
	// 0.4 and 0.6 are really just guesses, but they seem pretty good.
	// (0.4 to 0.6 is a fence that's 0.2 wide, down the center of the block)
	static constexpr double MIN_COORD = 0.4;
	static constexpr double MAX_COORD = 0.6;

	virtual cBoundingBox GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) const override
	{
		bool XMSolid = cBlockInfo::IsSolid(a_XM);
		bool XPSolid = cBlockInfo::IsSolid(a_XP);
		bool ZMSolid = cBlockInfo::IsSolid(a_ZM);
		bool ZPSolid = cBlockInfo::IsSolid(a_ZP);

		double FENCE_HEIGHT = cBlockInfo::GetBlockHeight(m_BlockType);

		// Entities can never be in the center
		cBoundingBox PlacementBox(MIN_COORD, MAX_COORD, 0, FENCE_HEIGHT, MIN_COORD, MAX_COORD);

		// For each solid neighbor, the hitbox extends that way
		if (XMSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(0, 0.5, 0, FENCE_HEIGHT, MIN_COORD, MAX_COORD));
		}
		if (XPSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(0.5, 1.0, 0, FENCE_HEIGHT, MIN_COORD, MAX_COORD));
		}
		if (ZMSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(MIN_COORD, MAX_COORD, 0, FENCE_HEIGHT, 0.0, 0.5));
		}
		if (ZPSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(MIN_COORD, MAX_COORD, 0, FENCE_HEIGHT, 0.5, 1.0));
		}

		// For each corner, fill in the corner
		if (XMSolid && ZMSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(0, 0.5, 0, FENCE_HEIGHT, 0, 0.5));
		}
		if (XPSolid && ZMSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(0.5, 1.0, 0, FENCE_HEIGHT, 0, 0.5));
		}
		if (XPSolid && ZPSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(0.5, 1.0, 0, FENCE_HEIGHT, 0.5, 1.0));
		}
		if (XMSolid && ZPSolid)
		{
			PlacementBox = PlacementBox.Union(cBoundingBox(0, 0.5, 0, FENCE_HEIGHT, 0.5, 1.0));
		}

		return PlacementBox;
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
		auto LeashKnot = cLeashKnot::FindKnotAtPos(*a_Player.GetWorld(), a_BlockPos);
		auto KnotAlreadyExists = (LeashKnot != nullptr);

		if (KnotAlreadyExists)
		{
			// Check leashed nearby mobs to leash them to the knot
			LeashKnot->TiePlayersLeashedMobs(a_Player, KnotAlreadyExists);
		}
		// New knot? needs to init and produce sound effect
		else
		{
			auto NewLeashKnot = std::make_unique<cLeashKnot>(a_BlockFace, a_BlockPos);
			auto NewLeashKnotPtr = NewLeashKnot.get();

			NewLeashKnotPtr->TiePlayersLeashedMobs(a_Player, KnotAlreadyExists);

			// Only put the knot in the world if any mob has been leashed to
			if (NewLeashKnotPtr->HasAnyMobLeashed())
			{
				if (!NewLeashKnotPtr->Initialize(std::move(NewLeashKnot), *a_Player.GetWorld()))
				{
					return false;
				}
				a_Player.GetWorld()->BroadcastSoundEffect("entity.leashknot.place", a_Player.GetPosition(), 1, 1);
			}
			else
			{
				return false;
			}
		}

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





	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override
	{
		UNUSED(a_Digger);
		// Destroy any leash knot tied to the fence:
		auto leashKnot = cLeashKnot::FindKnotAtPos(a_WorldInterface, a_BlockPos);
		if (leashKnot != nullptr)
		{
			leashKnot->SetShouldSelfDestroy();
		}
	}
};




