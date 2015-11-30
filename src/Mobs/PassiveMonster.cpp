
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





void cPassiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_EMState == ESCAPING)
	{
		CheckEventLostPlayer();
	}
	if ((m_LovePartner != nullptr) && m_LovePartner->IsDestroyed())
	{
		m_LovePartner = nullptr;
	}
	if (m_LovePartner != nullptr)
	{
		// if we have a partner, bump into them until baby is made
		if (m_MatingTimer > 0)
		{
			Vector3d Pos = m_LovePartner->GetPosition();
			MoveToPosition(Pos);
		}
		else
		{
			// spawn baby
			Vector3f Pos = (GetPosition() + m_LovePartner->GetPosition()) * 0.5;
			m_World->SpawnMob(Pos.x, Pos.y, Pos.z, GetMobType(), true);

			cFastRandom Random;
			m_World->SpawnExperienceOrb(Pos.x, Pos.y, Pos.z, 1 + Random.NextInt(6));

			m_LovePartner->ResetLoveMode();
			ResetLoveMode();
		}
	}
	else
	{
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
					// if we're the same species as someone around and they dont have a partner, swipe right
					if ((a_Entity->GetEntityType() == m_Me->GetEntityType()) && (a_Entity != m_Me))
					{
						cPassiveMonster * Me = static_cast<cPassiveMonster*>(m_Me);
						cPassiveMonster * Partner = static_cast<cPassiveMonster*>(a_Entity);
						if (Partner->IsInLove() && (Partner->GetPartner() == nullptr))
						{
							Partner->EngageLoveMode(Me);
							Me->EngageLoveMode(Partner);
							return true;
						}
					}
					return false;
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

	// if right clicked on the player with breeding items, go into lovemode
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



