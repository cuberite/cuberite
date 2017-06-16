
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveMonster.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BoundingBox.h"




cPassiveMonster::cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height),
	m_LovePartner(nullptr),
	m_LoveTimer(0),
	m_LoveCooldown(0),
	m_MatingTimer(0)
{
	m_EMPersonality = PASSIVE;
}





bool cPassiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}
	if ((a_TDI.Attacker != this) && (a_TDI.Attacker != nullptr))
	{
		m_EMState = ESCAPING;
	}
	return true;
}





void cPassiveMonster::EngageLoveMode(cPassiveMonster * a_Partner)
{
	m_LovePartner = a_Partner;
	m_MatingTimer = 50;  // about 3 seconds of mating
}





void cPassiveMonster::ResetLoveMode()
{
	m_LovePartner = nullptr;
	m_LoveTimer = 0;
	m_MatingTimer = 0;
	m_LoveCooldown = 20 * 60 * 5;  // 5 minutes

	// when an animal is in love mode, the client only stops sending the hearts if we let them know it's in cooldown, which is done with the "age" metadata
	m_World->BroadcastEntityMetadata(*this);
}





void cPassiveMonster::Destroyed()
{
	if (m_LovePartner != nullptr)
	{
		m_LovePartner->ResetLoveMode();
	}
	super::Destroyed();
}





void cPassiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_EMState == ESCAPING)
	{
		CheckEventLostPlayer();
	}

	// if we have a partner, mate
	if (m_LovePartner != nullptr)
	{

		if (m_MatingTimer > 0)
		{
			// If we should still mate, keep bumping into them until baby is made
			Vector3d Pos = m_LovePartner->GetPosition();
			MoveToPosition(Pos);
		}
		else
		{
			// Mating finished. Spawn baby
			Vector3f Pos = (GetPosition() + m_LovePartner->GetPosition()) * 0.5;
			UInt32 BabyID = m_World->SpawnMob(Pos.x, Pos.y, Pos.z, GetMobType(), true);

			class cBabyInheritCallback :
				public cEntityCallback
			{
			public:
				cPassiveMonster * Baby;
				cBabyInheritCallback() : Baby(nullptr) { }
				virtual bool Item(cEntity * a_Entity) override
				{
					Baby = static_cast<cPassiveMonster *>(a_Entity);
					return true;
				}
			} Callback;

			m_World->DoWithEntityByID(BabyID, Callback);
			if (Callback.Baby != nullptr)
			{
				Callback.Baby->InheritFromParents(this, m_LovePartner);
			}

			m_World->SpawnExperienceOrb(Pos.x, Pos.y, Pos.z, GetRandomProvider().RandInt(1, 6));

			m_LovePartner->ResetLoveMode();
			ResetLoveMode();
		}
	}
	else
	{
		// We have no partner, so we just chase the player if they have our breeding item
		cItems FollowedItems;
		GetFollowedItems(FollowedItems);
		if (FollowedItems.Size() > 0)
		{
			cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), static_cast<float>(m_SightDistance));
			if (a_Closest_Player != nullptr)
			{
				cItem EquippedItem = a_Closest_Player->GetEquippedItem();
				if (FollowedItems.ContainsType(EquippedItem))
				{
					Vector3d PlayerPos = a_Closest_Player->GetPosition();
					MoveToPosition(PlayerPos);
				}
			}
		}
	}

	// If we are in love mode but we have no partner, search for a partner neabry
	if (m_LoveTimer > 0)
	{
		if (m_LovePartner == nullptr)
		{
			class LookForLover : public cEntityCallback
			{
			public:
				cEntity * m_Me;

				LookForLover(cEntity * a_Me) :
					m_Me(a_Me)
				{
				}

				virtual bool Item(cEntity * a_Entity) override
				{
					// If the entity is not a monster, don't breed with it
					// Also, do not self-breed
					if ((a_Entity->GetEntityType() != etMonster) || (a_Entity == m_Me))
					{
						return false;
					}

					cPassiveMonster * Me = static_cast<cPassiveMonster*>(m_Me);
					cPassiveMonster * PotentialPartner = static_cast<cPassiveMonster*>(a_Entity);

					// If the potential partner is not of the same species, don't breed with it
					if (PotentialPartner->GetMobType() != Me->GetMobType())
					{
						return false;
					}

					// If the potential partner is not in love
					// Or they already have a mate, do not breed with them
					if ((!PotentialPartner->IsInLove()) || (PotentialPartner->GetPartner() != nullptr))
					{
						return false;
					}

					// All conditions met, let's breed!
					PotentialPartner->EngageLoveMode(Me);
					Me->EngageLoveMode(PotentialPartner);
					return true;
				}
			} Callback(this);

			m_World->ForEachEntityInBox(cBoundingBox(GetPosition(), 8, 8), Callback);
		}

		m_LoveTimer--;
	}
	if (m_MatingTimer > 0)
	{
		m_MatingTimer--;
	}
	if (m_LoveCooldown > 0)
	{
		m_LoveCooldown--;
	}
}





void cPassiveMonster::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	// If a player holding breeding items right-clicked me, go into love mode
	if ((m_LoveCooldown == 0) && !IsInLove() && !IsBaby())
	{
		short HeldItem = a_Player.GetEquippedItem().m_ItemType;
		cItems Items;
		GetBreedingItems(Items);
		if (Items.ContainsType(HeldItem))
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}
			m_LoveTimer = 20 * 30;  // half a minute
			m_World->BroadcastEntityStatus(*this, esMobInLove);
		}
	}
}



