
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
		// this vector allows to store the reference to an existing knot, if found inside the callback function
		std::vector<cLeashKnot *> LeashKnotList;

		// check if already exists a knot there
		class LookForKnots : public cEntityCallback
		{
		public:
			std::vector<cLeashKnot *> * m_LeashKnotList;

			LookForKnots(std::vector<cLeashKnot *> * a_LeashKnotList) :
				m_LeashKnotList(a_LeashKnotList)
			{
			}

			virtual bool Item(cEntity * a_Entity) override
			{
				if (a_Entity->IsLeashKnot())
				{
					m_LeashKnotList->push_back(reinterpret_cast<cLeashKnot *>(a_Entity));
					return true;  // this makes ForEachEntityInBox call to return false, so we can know if a knot was found without checking list size
				}
				return false;
			}
		} CallbackFindKnot(& LeashKnotList);
		bool AlreadyExistsAKnot = !a_Player->GetWorld()->ForEachEntityInBox(cBoundingBox(new Vector3d(a_BlockX, a_BlockY, a_BlockZ), 0.5, 1), CallbackFindKnot);

		// Reuse / create the leash knot
		cLeashKnot * LeashKnot = (AlreadyExistsAKnot ? LeashKnotList.front() : new cLeashKnot(a_BlockFace, a_BlockX, a_BlockY, a_BlockZ));

		// Check leashed nearby mobs to leash them to the knot
		class LookForLeasheds : public cEntityCallback
		{
		public:
			cLeashKnot * m_Knot;
			cPlayer * m_Player;
			bool m_KnotExists;

			LookForLeasheds(cLeashKnot * a_Knot, cPlayer * a_PlayerLeashedTo, bool a_KnotExists) :
				m_Knot(a_Knot),
				m_Player(a_PlayerLeashedTo),
				m_KnotExists(a_KnotExists)
			{
			}

			virtual bool Item(cEntity * a_Entity) override
			{
				// If the entity is not a monster skip it
				if (a_Entity->GetEntityType() != cEntity::eEntityType::etMonster)
				{
					return false;
				}

				cMonster * PotentialLeashed = static_cast<cMonster*>(a_Entity);

				// If it's not leasheble skip it
				if (!PotentialLeashed->CanBeLeashed())
				{
					return false;
				}

				// If it's not leashed to the player skip it
				if (
					!PotentialLeashed->IsLeashed() ||
					!PotentialLeashed->GetLeashedTo()->IsPlayer() ||
					(PotentialLeashed->GetLeashedTo()->GetUniqueID() != m_Player->GetUniqueID())
				)
				{
					return false;
				}

				// All conditions met, unleash from player and leash to fence
				PotentialLeashed->GetLeashedTo()->RemoveLeashedMob(PotentialLeashed, false, false);
				m_Knot->AddLeashedMob(PotentialLeashed, m_KnotExists);
				return false;
			}
		} Callback(LeashKnot, a_Player, AlreadyExistsAKnot);
		a_Player->GetWorld()->ForEachEntityInBox(cBoundingBox(LeashKnot->GetPosition(), 8, 8), Callback);

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
				LOGD("No mob found to leash, destroying knot");
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

};




