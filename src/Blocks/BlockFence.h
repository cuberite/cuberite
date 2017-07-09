
#pragma once

#include "BlockHandler.h"
#include "MetaRotator.h"
#include "../EffectID.h"
#include "Entities/LeashKnot.h"
#include "BoundingBox.h"
#include "../Mobs/PassiveMonster.h"



class cBlockFenceHandler :
	public cMetaRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01, true>
{
public:
	cBlockFenceHandler(BLOCKTYPE a_BlockType) :
		cMetaRotator<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01, true>(a_BlockType)
	{
	}

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		cLeashKnot * LeashKnot = nullptr;

		// Check if already exists a knot there
		LookForKnot CallbackFindKnot = LookForKnot(&LeashKnot);
		bool AlreadyExistsAKnot = !a_Player->GetWorld()->ForEachEntityInBox(cBoundingBox(new Vector3d(a_BlockX, a_BlockY, a_BlockZ), 0.5, 1), CallbackFindKnot);

		// Reuse / create the leash knot
		if (LeashKnot == nullptr)
		{
			LeashKnot = new cLeashKnot(a_BlockFace, a_BlockX, a_BlockY, a_BlockZ);
		}

		// Check leashed nearby mobs to leash them to the knot
		LeashKnot->TieNearbyMobs(*a_Player, AlreadyExistsAKnot);

		// New knot? needs to init and produce sound effect
		if (!AlreadyExistsAKnot)
		{
			// Only put the knot in the world if any mob has been leashed to
			if (LeashKnot->HasAnyMobLeashed())
			{
				if (!LeashKnot->Initialize(*a_Player->GetWorld()))
				{
					delete LeashKnot;
					LeashKnot = nullptr;
					return false;
				}
				a_Player->GetWorld()->BroadcastSoundEffect("entity.leashknot.place", a_Player->GetPosX(), a_Player->GetPosY(), a_Player->GetPosZ(), 1, 1);
			}
			else
			{
				delete LeashKnot;
				LeashKnot = nullptr;
				return false;
			}
		}

		return true;
	}

	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		cLeashKnot * LeashKnotFound = nullptr;
		LookForKnot CallbackFindKnot = LookForKnot(&LeashKnotFound);
		a_Player->GetWorld()->ForEachEntityInBox(cBoundingBox(new Vector3d(a_BlockX, a_BlockY, a_BlockZ), 2.0, 1), CallbackFindKnot);

		if (LeashKnotFound != nullptr)
		{
			LeashKnotFound->SetShouldSelfDestroy();
		}
	}

protected:

	class LookForKnot : public cEntityCallback
	{
	public:
		cLeashKnot ** m_LeashKnot;

		LookForKnot(cLeashKnot ** a_LeashKnot) :
			m_LeashKnot(a_LeashKnot)
		{
		}

		virtual bool Item(cEntity * a_Entity) override
		{
			if (a_Entity->IsLeashKnot())
			{
				*m_LeashKnot = reinterpret_cast<cLeashKnot *>(a_Entity);
				return true;  // this makes ForEachEntityInBox call to return false, so we can know if a knot was found
			}
			return false;
		}
	};

};




