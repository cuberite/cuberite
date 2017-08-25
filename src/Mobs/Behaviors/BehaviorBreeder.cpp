#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorBreeder.h"
#include "../PassiveMonster.h"
#include "../../World.h"
#include "../Monster.h"
#include "../../Entities/Player.h"
#include "../../Item.h"
#include "../../BoundingBox.h"

cBehaviorBreeder::cBehaviorBreeder() :
	m_LovePartner(nullptr),
	m_LoveTimer(0),
	m_LoveCooldown(0),
	m_MatingTimer(0)
{
}





void cBehaviorBreeder::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachTickBehavior(this);
	m_Parent->AttachPostTickBehavior(this);
	m_Parent->AttachRightClickBehavior(this);
	m_Parent->AttachDestroyBehavior(this);
}





void cBehaviorBreeder::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	LOGD("mobDebug - Behavior Breeder: Tick");
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	cWorld * World = m_Parent->GetWorld();
	// if we have a partner, mate
	if (m_LovePartner != nullptr)
	{
		if (m_MatingTimer > 0)
		{
			// If we should still mate, keep bumping into them until baby is made
			Vector3d Pos = m_LovePartner->GetPosition();
			m_Parent->MoveToPosition(Pos);
		}
		else
		{
			// Mating finished. Spawn baby
			Vector3f Pos = (m_Parent->GetPosition() + m_LovePartner->GetPosition()) * 0.5;
			UInt32 BabyID = World->SpawnMob(Pos.x, Pos.y, Pos.z, m_Parent->GetMobType(), true);

			class cBabyInheritCallback :
				public cEntityCallback
			{
			public:
				cMonster * Baby;
				cBabyInheritCallback() : Baby(nullptr) { }
				virtual bool Item(cEntity * a_Entity) override
				{
					Baby = static_cast<cMonster *>(a_Entity);
					return true;
				}
			} Callback;

			m_Parent->GetWorld()->DoWithEntityByID(BabyID, Callback);
			if (Callback.Baby != nullptr)
			{
				Callback.Baby->InheritFromParents(m_Parent, m_LovePartner);
			}

			cFastRandom Random;
			World->SpawnExperienceOrb(Pos.x, Pos.y, Pos.z, 1 + (Random.RandInt() % 6));

			m_LovePartner->GetBehaviorBreeder()->ResetLoveMode();
			ResetLoveMode();
		}
	}
}





void cBehaviorBreeder::PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	if (m_MatingTimer > 0)
	{
		m_MatingTimer--;
	}
	if (m_LoveCooldown > 0)
	{
		m_LoveCooldown--;
	}
	if (m_LoveTimer > 0)
	{
		m_LoveTimer--;
	}
}





bool cBehaviorBreeder::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	cWorld * World = m_Parent->GetWorld();

	// if we have a love partner, we should control the mob
	if (m_LovePartner != nullptr)
	{
		return true;
	}

	// If we are in love mode and we have no partner, try to find one
	if (m_LoveTimer > 0)
	{
		class LookForLover : public cEntityCallback
		{
		public:
			cMonster * m_Me;
			LookForLover(cMonster * a_Me) :
				m_Me(a_Me)
			{
			}

			virtual bool Item(cEntity * a_Entity) override
			{
				// If the entity is not a monster, don't breed with it
				// Also, do not self-breed
				if ((a_Entity->GetEntityType() != cEntity::eEntityType::etMonster) || (a_Entity == m_Me))
				{
					return false;
				}

				auto PotentialPartner = static_cast<cMonster*>(a_Entity);

				// If the potential partner is not of the same species, don't breed with it
				if (PotentialPartner->GetMobType() != m_Me->GetMobType())
				{
					return false;
				}

				auto PartnerBreedingBehavior = PotentialPartner->GetBehaviorBreeder();
				auto MyBreedingBehavior = m_Me->GetBehaviorBreeder();

				// If the potential partner is not in love
				// Or they already have a mate, do not breed with them

				if ((!PartnerBreedingBehavior->IsInLove()) || (PartnerBreedingBehavior->GetPartner() != nullptr))
				{
					return false;
				}

				// All conditions met, let's breed!
				PartnerBreedingBehavior->EngageLoveMode(m_Me);
				MyBreedingBehavior->EngageLoveMode(PotentialPartner);
				return true;
			}
		} Callback(m_Parent);

		World->ForEachEntityInBox(cBoundingBox(m_Parent->GetPosition(), 8, 8), Callback);
		if (m_LovePartner != nullptr)
		{
			return true;  // We found love and took control of the monster, prevent other Behaviors from doing so
		}
	}

	return false;
}

void cBehaviorBreeder::Destroyed()
{
	LOGD("mobDebug - Behavior Breeder: Destroyed");
	if (m_LovePartner != nullptr)
	{
		m_LovePartner->GetBehaviorBreeder()->ResetLoveMode();
	}
}





void cBehaviorBreeder::OnRightClicked(cPlayer & a_Player)
{
	// If a player holding breeding items right-clicked me, go into love mode
	if ((m_LoveCooldown == 0) && !IsInLove() && !m_Parent->IsBaby())
	{
		short HeldItem = a_Player.GetEquippedItem().m_ItemType;
		cItems BreedingItems;
		m_Parent->GetFollowedItems(BreedingItems);
		if (BreedingItems.ContainsType(HeldItem))
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}
			m_LoveTimer = 20 * 30;  // half a minute
			m_Parent->GetWorld()->BroadcastEntityStatus(*m_Parent, cEntity::eEntityStatus::esMobInLove);
		}
	}
}



void cBehaviorBreeder::EngageLoveMode(cMonster * a_Partner)
{
	m_LovePartner = a_Partner;
	m_MatingTimer = 50;  // about 3 seconds of mating
}





void cBehaviorBreeder::ResetLoveMode()
{
	m_LovePartner = nullptr;
	m_LoveTimer = 0;
	m_MatingTimer = 0;
	m_LoveCooldown = 20 * 60 * 5;  // 5 minutes

	// when an animal is in love mode, the client only stops sending the hearts if we let them know it's in cooldown, which is done with the "age" metadata
	m_Parent->GetWorld()->BroadcastEntityMetadata(*m_Parent);
}





bool cBehaviorBreeder::IsInLove() const
{
	return m_LoveTimer > 0;
}





bool cBehaviorBreeder::IsInLoveCooldown() const
{
	return (m_LoveCooldown > 0);
}
