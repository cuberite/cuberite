#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Horse.h"
#include "../World.h"
#include "../EffectID.h"
#include "../Entities/Player.h"
#include "../UI/HorseWindow.h"





cHorse::cHorse(int Type, int Color, int Style, int TameTimes) :
	Super("Horse", mtHorse, "entity.horse.hurt", "entity.horse.death", "entity.horse.ambient", 1.4, 1.6),
	cEntityWindowOwner(this),
	m_bHasChest(false),
	m_bIsEating(false),
	m_bIsRearing(false),
	m_bIsMouthOpen(false),
	m_bIsTame(false),
	m_Type(Type),
	m_Color(Color),
	m_Style(Style),
	m_TimesToTame(TameTimes),
	m_TameAttemptTimes(0),
	m_RearTickCount(0),
	m_MaxSpeed(14.0)
{
}





cHorse::~cHorse()
{
	auto Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cHorse::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
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
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, GetPosition().Floor(), int(SmokeDirection::SOUTH_EAST));
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, GetPosition().Floor(), int(SmokeDirection::SOUTH_WEST));
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, GetPosition().Floor(), int(SmokeDirection::NORTH_EAST));
				m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, GetPosition().Floor(), int(SmokeDirection::NORTH_WEST));

				m_World->BroadcastSoundEffect("entity.horse.angry", GetPosition(), 1.0f, 1.0f);
				m_Attachee->Detach();
				m_bIsRearing = true;
			}
		}
		else
		{
			m_World->BroadcastParticleEffect("heart", static_cast<Vector3f>(GetPosition()), Vector3f{}, 0, 5);
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
	Super::OnRightClicked(a_Player);

	if (m_bIsTame)
	{
		if (a_Player.IsCrouched())
		{
			PlayerOpenWindow(a_Player);
			return;
		}

		auto EquipedItemType = a_Player.GetEquippedItem().m_ItemType;

		if (
			!IsSaddled() &&
			(
				(EquipedItemType == E_ITEM_SADDLE) ||
				ItemCategory::IsHorseArmor(EquipedItemType)
			)
		)
		{
			// Player is holding a horse inventory item, open the window:
			PlayerOpenWindow(a_Player);
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
		m_World->BroadcastSoundEffect("entity.horse.angry", GetPosition(), 1.0f, 0.8f);
	}
}





void cHorse::SetHorseSaddle(cItem a_Saddle)
{
	if (a_Saddle.m_ItemType == E_ITEM_SADDLE)
	{
		m_World->BroadcastSoundEffect("entity.horse.saddle", GetPosition(), 1.0f, 0.8f);
	}
	else if (!a_Saddle.IsEmpty())
	{
		return;  // Invalid item
	}

	m_Saddle = std::move(a_Saddle);
	m_World->BroadcastEntityMetadata(*this);
}





void cHorse::SetHorseArmor(cItem a_Armor)
{
	if (ItemCategory::IsHorseArmor(a_Armor.m_ItemType))
	{
		m_World->BroadcastSoundEffect("entity.horse.armor", GetPosition(), 1.0f, 0.8f);
	}
	else if (!a_Armor.IsEmpty())
	{
		return;  // Invalid item
	}

	m_Armor = std::move(a_Armor);
	m_World->BroadcastEntityMetadata(*this);
}





int cHorse::GetHorseArmour(void) const
{
	switch (m_Armor.m_ItemType)
	{
		case E_ITEM_EMPTY:               return 0;
		case E_ITEM_IRON_HORSE_ARMOR:    return 1;
		case E_ITEM_GOLD_HORSE_ARMOR:    return 2;
		case E_ITEM_DIAMOND_HORSE_ARMOR: return 3;

		default:
		{
			LOGWARN("cHorse::GetHorseArmour: Invalid armour item (%d)", m_Armor.m_ItemType);
			return 0;
		}
	}
}





void cHorse::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (IsBaby())
	{
		return;  // Babies don't drop items
	}

	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	if (IsSaddled())
	{
		a_Drops.push_back(m_Saddle);
	}
	if (!m_Armor.IsEmpty())
	{
		a_Drops.push_back(m_Armor);
	}
}





void cHorse::InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	// If horse is tame and someone is sitting on it, don't walk around
	if ((!m_bIsTame) || (m_Attachee == nullptr))
	{
		Super::InStateIdle(a_Dt, a_Chunk);
	}
}





void cHorse::HandleSpeedFromAttachee(float a_Forward, float a_Sideways)
{
	if ((m_bIsTame) && IsSaddled())
	{
		Super::HandleSpeedFromAttachee(a_Forward * m_MaxSpeed, a_Sideways * m_MaxSpeed);
	}
}





void cHorse::PlayerOpenWindow(cPlayer & a_Player)
{
	auto Window = GetWindow();
	if (Window == nullptr)
	{
		Window = new cHorseWindow(*this);
		OpenWindow(Window);
	}

	a_Player.OpenWindow(*Window);
}
