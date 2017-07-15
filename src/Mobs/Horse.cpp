#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Horse.h"
#include "../World.h"
#include "../EffectID.h"
#include "../Entities/Player.h"
#include "Broadcaster.h"





cHorse::cHorse(int Type, int Color, int Style, int TameTimes) :
	super("Horse", mtHorse, "entity.horse.hurt", "entity.horse.death", 1.4, 1.6),
	m_bHasChest(false),
	m_bIsEating(false),
	m_bIsRearing(false),
	m_bIsMouthOpen(false),
	m_bIsTame(false),
	m_bIsSaddled(false),
	m_Type(Type),
	m_Color(Color),
	m_Style(Style),
	m_Armour(0),
	m_TimesToTame(TameTimes),
	m_TameAttemptTimes(0),
	m_RearTickCount(0),
	m_MaxSpeed(14.0)
{
}





void cHorse::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	auto & Random = GetRandomProvider();

	if (!m_bIsMouthOpen)
	{
		if (Random.RandBool(0.02))
		{
			m_bIsMouthOpen = true;
		}
	}
	else
	{
		if (Random.RandBool(0.10))
		{
			m_bIsMouthOpen = false;
		}
	}

	if ((m_Attachee != nullptr) && (!m_bIsTame))
	{
		if (m_TameAttemptTimes < m_TimesToTame)
		{
			if (Random.RandBool(0.02))
			{
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), int(SmokeDirection::SOUTH_EAST));
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), int(SmokeDirection::SOUTH_WEST));
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), int(SmokeDirection::NORTH_EAST));
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), int(SmokeDirection::NORTH_WEST));

				m_World->BroadcastSoundEffect("entity.horse.angry", GetPosX(), GetPosY(), GetPosZ(), 1.0f, 1.0f);
				m_Attachee->Detach();
				m_bIsRearing = true;
			}
		}
		else
		{
			m_World->GetBroadcaster().BroadcastParticleEffect("heart", static_cast<Vector3f>(GetPosition()), Vector3f{}, 0, 5);
			m_bIsTame = true;
		}
	}

	if (m_bIsRearing)
	{
		if (m_RearTickCount == 20)
		{
			m_bIsRearing = false;
			m_RearTickCount = 0;
		}
		else
		{
			m_RearTickCount++;
		}
	}

	m_World->BroadcastEntityMetadata(*this);
}





void cHorse::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	if (m_bIsTame)
	{
		if (!m_bIsSaddled)
		{
			if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_SADDLE)
			{
				// Saddle the horse:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.GetInventory().RemoveOneEquippedItem();
				}
				m_bIsSaddled = true;
				m_World->BroadcastEntityMetadata(*this);
			}
			else
			{
				a_Player.AttachTo(this);
			}
		}
		else
		{
			a_Player.AttachTo(this);
		}
	}
	else if (a_Player.GetEquippedItem().IsEmpty())
	{
		// Check if leashed / unleashed to player before try to ride
		if (!m_IsLeashActionJustDone)
		{
			if (m_Attachee != nullptr)
			{
				if (m_Attachee->GetUniqueID() == a_Player.GetUniqueID())
				{
					a_Player.Detach();
					return;
				}

				if (m_Attachee->IsPlayer())
				{
					return;
				}

				m_Attachee->Detach();
			}

			m_TameAttemptTimes++;
			a_Player.AttachTo(this);
		}
	}
	else
	{
		m_bIsRearing = true;
		m_RearTickCount = 0;
		m_World->BroadcastSoundEffect("entity.horse.angry", GetPosX(), GetPosY(), GetPosZ(), 1.0f, 0.8f);
	}
}





void cHorse::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	if (m_bIsSaddled)
	{
		a_Drops.push_back(cItem(E_ITEM_SADDLE, 1));
	}
}





void cHorse::InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	// If horse is tame and someone is sitting on it, don't walk around
	if ((!m_bIsTame) || (m_Attachee == nullptr))
	{
		super::InStateIdle(a_Dt, a_Chunk);
	}
}





void cHorse::HandleSpeedFromAttachee(float a_Forward, float a_Sideways)
{
	if ((m_bIsTame) && (m_bIsSaddled))
	{
		super::HandleSpeedFromAttachee(a_Forward * m_MaxSpeed, a_Sideways * m_MaxSpeed);
	}
}
