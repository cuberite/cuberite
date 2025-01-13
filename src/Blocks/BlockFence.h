
#pragma once

#include "BlockHandler.h"
#include "../BoundingBox.h"
#include "ChunkInterface.h"
#include "../EffectID.h"
#include "../Entities/LeashKnot.h"
#include "../BoundingBox.h"
#include "../Mobs/PassiveMonster.h"
#include "AllTags/BlockTags.h"


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
			case BlockType::BambooFence:
			case BlockType::CherryFence:
			case BlockType::CrimsonFence:
			case BlockType::MangroveFence:
			case BlockType::PaleOakFence:
				return true;
			default: return false;
		}
	}

private:

	// These are the min and max coordinates (X and Z) for a straight fence.
	// 0.4 and 0.6 are really just guesses, but they seem pretty good.
	// (0.4 to 0.6 is a fence that's 0.2 wide, down the center of the block)
	static constexpr double MIN_COORD = 0.4;
	static constexpr double MAX_COORD = 0.6;

	virtual cBoundingBox GetPlacementCollisionBox(BlockState a_XM, BlockState a_XP, BlockState a_YM, BlockState a_YP, BlockState a_ZM, BlockState a_ZP) const override
	{
		bool XMSolid = cBlockInfo::IsSolid(a_XM);
		bool XPSolid = cBlockInfo::IsSolid(a_XP);
		bool ZMSolid = cBlockInfo::IsSolid(a_ZM);
		bool ZPSolid = cBlockInfo::IsSolid(a_ZP);

		// This is a bit of a hack - some blocks change their height depending on their state so we can't only switch on the type.
		// We use an example fence: all fences have the same height.
		double FENCE_HEIGHT = cBlockInfo::GetBlockHeight(Block::OakFence::OakFence());

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
		BlockState a_OldBlock,
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


#define GET_FENCE_XM(FenceType, State) \
	ToPlace = Block::FenceType::FenceType( \
	Block::FenceType::East(current), \
	Block::FenceType::North(current), \
	Block::FenceType::South(current), \
	Block::FenceType::Waterlogged(current), \
	State); break;
#define GET_FENCE_XP(FenceType, State) \
	ToPlace = Block::FenceType::FenceType( \
	State, \
	Block::FenceType::North(current), \
	Block::FenceType::South(current), \
	Block::FenceType::Waterlogged(current), \
	Block::FenceType::West(current)); break;
#define GET_FENCE_ZM(FenceType, State) \
	ToPlace = Block::FenceType::FenceType( \
	Block::FenceType::East(current), \
	State, \
	Block::FenceType::South(current), \
	Block::FenceType::Waterlogged(current), \
	Block::FenceType::West(current)); break;
#define GET_FENCE_ZP(FenceType, State) \
	ToPlace = Block::FenceType::FenceType( \
	Block::FenceType::East(current), \
	Block::FenceType::North(current), \
	State, \
	Block::FenceType::Waterlogged(current), \
	Block::FenceType::West(current)); break;


	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		BlockState current = a_ChunkInterface.GetBlock(a_BlockPos);
		bool NewState = false;
		BlockState ToPlace;
		switch (a_WhichNeighbor)
		{
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			case BLOCK_FACE_NONE: return;
			case BLOCK_FACE_XM:
			NewState = BlockTags::Fences(a_ChunkInterface.GetBlock(a_BlockPos.addedX(-1)).Type());
			switch (m_BlockType)
			{
				case BlockType::OakFence: GET_FENCE_XM(OakFence, NewState)
				case BlockType::AcaciaFence: GET_FENCE_XM(AcaciaFence, NewState)
				case BlockType::BirchFence: GET_FENCE_XM(BirchFence, NewState)
				case BlockType::BambooFence: GET_FENCE_XM(BambooFence, NewState)
				case BlockType::DarkOakFence: GET_FENCE_XM(DarkOakFence, NewState)
				case BlockType::JungleFence: GET_FENCE_XM(JungleFence, NewState)
				case BlockType::CrimsonFence: GET_FENCE_XM(CrimsonFence, NewState)
				case BlockType::CherryFence: GET_FENCE_XM(CherryFence, NewState)
				case BlockType::MangroveFence: GET_FENCE_XM(MangroveFence, NewState)
				case BlockType::NetherBrickFence: GET_FENCE_XM(NetherBrickFence, NewState)
				case BlockType::PaleOakFence: GET_FENCE_XM(PaleOakFence, NewState)
				case BlockType::SpruceFence: GET_FENCE_XM(SpruceFence, NewState)
				case BlockType::WarpedFence: GET_FENCE_XM(WarpedFence, NewState)
			}
			break;
			case BLOCK_FACE_XP:
			NewState = BlockTags::Fences(a_ChunkInterface.GetBlock(a_BlockPos.addedX(1)).Type());
			switch (m_BlockType)
			{
				case BlockType::OakFence: GET_FENCE_XP(OakFence, NewState)
				case BlockType::AcaciaFence: GET_FENCE_XP(AcaciaFence, NewState)
				case BlockType::BirchFence: GET_FENCE_XP(BirchFence, NewState)
				case BlockType::BambooFence: GET_FENCE_XP(BambooFence, NewState)
				case BlockType::DarkOakFence: GET_FENCE_XP(DarkOakFence, NewState)
				case BlockType::JungleFence: GET_FENCE_XP(JungleFence, NewState)
				case BlockType::CrimsonFence: GET_FENCE_XP(CrimsonFence, NewState)
				case BlockType::CherryFence: GET_FENCE_XP(CherryFence, NewState)
				case BlockType::MangroveFence: GET_FENCE_XP(MangroveFence, NewState)
				case BlockType::NetherBrickFence: GET_FENCE_XP(NetherBrickFence, NewState)
				case BlockType::PaleOakFence: GET_FENCE_XP(PaleOakFence, NewState)
				case BlockType::SpruceFence: GET_FENCE_XP(SpruceFence, NewState)
				case BlockType::WarpedFence: GET_FENCE_XP(WarpedFence, NewState)
			}
			break;
			case BLOCK_FACE_ZM:
			NewState = BlockTags::Fences(a_ChunkInterface.GetBlock(a_BlockPos.addedZ(-1)).Type());
			switch (m_BlockType)
			{
				case BlockType::OakFence: GET_FENCE_ZM(OakFence, NewState)
				case BlockType::AcaciaFence: GET_FENCE_ZM(AcaciaFence, NewState)
				case BlockType::BirchFence: GET_FENCE_ZM(BirchFence, NewState)
				case BlockType::BambooFence: GET_FENCE_ZM(BambooFence, NewState)
				case BlockType::DarkOakFence: GET_FENCE_ZM(DarkOakFence, NewState)
				case BlockType::JungleFence: GET_FENCE_ZM(JungleFence, NewState)
				case BlockType::CrimsonFence: GET_FENCE_ZM(CrimsonFence, NewState)
				case BlockType::CherryFence: GET_FENCE_ZM(CherryFence, NewState)
				case BlockType::MangroveFence: GET_FENCE_ZM(MangroveFence, NewState)
				case BlockType::NetherBrickFence: GET_FENCE_ZM(NetherBrickFence, NewState)
				case BlockType::PaleOakFence: GET_FENCE_ZM(PaleOakFence, NewState)
				case BlockType::SpruceFence: GET_FENCE_ZM(SpruceFence, NewState)
				case BlockType::WarpedFence: GET_FENCE_ZM(WarpedFence, NewState)
			}
			break;
			case BLOCK_FACE_ZP:
			NewState = BlockTags::Fences(a_ChunkInterface.GetBlock(a_BlockPos.addedZ(1)).Type());
			switch (m_BlockType)
			{
				case BlockType::OakFence: GET_FENCE_ZP(OakFence, NewState)
				case BlockType::AcaciaFence: GET_FENCE_ZP(AcaciaFence, NewState)
				case BlockType::BirchFence: GET_FENCE_ZP(BirchFence, NewState)
				case BlockType::BambooFence: GET_FENCE_ZP(BambooFence, NewState)
				case BlockType::DarkOakFence: GET_FENCE_ZP(DarkOakFence, NewState)
				case BlockType::JungleFence: GET_FENCE_ZP(JungleFence, NewState)
				case BlockType::CrimsonFence: GET_FENCE_ZP(CrimsonFence, NewState)
				case BlockType::CherryFence: GET_FENCE_ZP(CherryFence, NewState)
				case BlockType::MangroveFence: GET_FENCE_ZP(MangroveFence, NewState)
				case BlockType::NetherBrickFence: GET_FENCE_ZP(NetherBrickFence, NewState)
				case BlockType::PaleOakFence: GET_FENCE_ZP(PaleOakFence, NewState)
				case BlockType::SpruceFence: GET_FENCE_ZP(SpruceFence, NewState)
				case BlockType::WarpedFence: GET_FENCE_ZP(WarpedFence, NewState)
			}
			break;
		}
		a_ChunkInterface.FastSetBlock(a_BlockPos, ToPlace);
	}
};




