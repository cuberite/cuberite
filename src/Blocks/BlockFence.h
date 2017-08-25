
#pragma once

#include "BlockHandler.h"
#include "../BoundingBox.h"
#include "../EffectID.h"
#include "Entities/LeashKnot.h"
#include "BoundingBox.h"
#include "../Mobs/PassiveMonster.h"



class cBlockFenceHandler :
	public cBlockHandler
{
public:
	// These are the min and max coordinates (X and Z) for a straight fence.
	// 0.4 and 0.6 are really just guesses, but they seem pretty good.
	// (0.4 to 0.6 is a fence that's 0.2 wide, down the center of the block)
	const double MIN_COORD = 0.4;
	const double MAX_COORD = 0.6;

	cBlockFenceHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual cBoundingBox GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) override
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

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		auto LeashKnot = cLeashKnot::FindKnotAtPos(*a_Player.GetWorld(), { a_BlockX, a_BlockY, a_BlockZ });
		auto KnotAlreadyExists = (LeashKnot != nullptr);

		if (KnotAlreadyExists)
		{
			// Check leashed nearby mobs to leash them to the knot
			LeashKnot->TiePlayersLeashedMobs(a_Player, KnotAlreadyExists);
		}
		// New knot? needs to init and produce sound effect
		else
		{
			auto NewLeashKnot = cpp14::make_unique<cLeashKnot>(a_BlockFace, a_BlockX, a_BlockY, a_BlockZ);
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

	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		auto LeashKnot = cLeashKnot::FindKnotAtPos(a_WorldInterface, { a_BlockX, a_BlockY, a_BlockZ });

		if (LeashKnot != nullptr)
		{
			LeashKnot->SetShouldSelfDestroy();
		}
	}

};




