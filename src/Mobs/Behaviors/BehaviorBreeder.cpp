#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorBreeder.h"
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
	m_Parent->m_BehaviorBreederPointer = this;
}





/*
 * CODE to be used AFTER the lambda merge
 * void cBehaviorBreeder::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
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

			cMonster * Baby = nullptr;

			m_Parent->GetWorld()->DoWithEntityByID(BabyID, [&](cEntity & a_Entity)
			{
				Baby = static_cast<cMonster *>(&a_Entity);
				return true;
			});

			if (Baby != nullptr)
			{
				Baby->InheritFromParents(m_Parent, m_LovePartner);
			}

			cFastRandom Random;
			World->SpawnExperienceOrb(Pos.x, Pos.y, Pos.z, 1 + (Random.RandInt() % 6));

			m_LovePartner->GetBehaviorBreeder()->ResetLoveMode();
			ResetLoveMode();
		}
	}
}*/







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





/*** CODE TO BE USED AFTER the lambda merge
 * bool cBehaviorBreeder::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
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
		m_Parent->GetWorld()->ForEachEntityInBox(cBoundingBox(m_Parent->GetPosition(), 8, 8), [=](cEntity & a_Entity)
		{
			// If the entity is not a monster, don't breed with it
			// Also, do not self-breed
			if ((a_Entity.GetEntityType() != cEntity::eEntityType::etMonster) || (&a_Entity == m_Parent))
			{
				return false;
			}

			auto PotentialPartner = static_cast<cMonster*>(&a_Entity);

			// If the potential partner is not of the same species, don't breed with it
			if (PotentialPartner->GetMobType() != m_Parent->GetMobType())
			{
				return false;
			}

			auto PartnerBreedingBehavior = PotentialPartner->GetBehaviorBreeder();
			auto MyBreedingBehavior = m_Parent->GetBehaviorBreeder();

			// If the potential partner is not in love
			// Or they already have a mate, do not breed with them

			if ((!PartnerBreedingBehavior->IsInLove()) || (PartnerBreedingBehavior->GetPartner() != nullptr))
			{
				return false;
			}

			// All conditions met, let's breed!
			PartnerBreedingBehavior->EngageLoveMode(m_Parent);
			MyBreedingBehavior->EngageLoveMode(PotentialPartner);
			return true;
		});


		if (m_LovePartner != nullptr)
		{
			return true;  // We found love and took control of the monster, prevent other Behaviors from doing so
		}
	}

	return false;
} **/

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
