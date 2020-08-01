#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EntityEffect.h"
#include "Player.h"
#include "../Chunk.h"
#include "../Mobs/Monster.h"





int cEntityEffect::GetPotionColor(short a_ItemDamage)
{
	// Lowest six bits
	return (a_ItemDamage & 0x3f);
}





cEntityEffect::eType cEntityEffect::GetPotionEffectType(short a_ItemDamage)
{
	// Lowest four bits
	// Potion effect bits are different from entity effect values
	// For reference: https://minecraft.gamepedia.com/Java_Edition_data_values#.22Potion_effect.22_bits
	switch (a_ItemDamage & 0x0f)
	{
		case 0x01: return cEntityEffect::effRegeneration;
		case 0x02: return cEntityEffect::effSpeed;
		case 0x03: return cEntityEffect::effFireResistance;
		case 0x04: return cEntityEffect::effPoison;
		case 0x05: return cEntityEffect::effInstantHealth;
		case 0x06: return cEntityEffect::effNightVision;
		case 0x08: return cEntityEffect::effWeakness;
		case 0x09: return cEntityEffect::effStrength;
		case 0x0a: return cEntityEffect::effSlowness;
		case 0x0b: return cEntityEffect::effJumpBoost;
		case 0x0c: return cEntityEffect::effInstantDamage;
		case 0x0d: return cEntityEffect::effWaterBreathing;
		case 0x0e: return cEntityEffect::effInvisibility;

		// No effect potions
		case 0x00:
		case 0x07:
		case 0x0f:
		{
			break;
		}
	}
	return cEntityEffect::effNoEffect;
}





short cEntityEffect::GetPotionEffectIntensity(short a_ItemDamage)
{
	// Level II potion if the fifth lowest bit is set
	return ((a_ItemDamage & 0x20) != 0) ? 1 : 0;
}





int cEntityEffect::GetPotionEffectDuration(short a_ItemDamage)
{
	// Base duration in ticks
	int base = 0;
	double TierCoeff = 1, ExtCoeff = 1, SplashCoeff = 1;

	switch (GetPotionEffectType(a_ItemDamage))
	{
		case cEntityEffect::effRegeneration:
		case cEntityEffect::effPoison:
		{
			base = 900;
			break;
		}

		case cEntityEffect::effSpeed:
		case cEntityEffect::effFireResistance:
		case cEntityEffect::effNightVision:
		case cEntityEffect::effStrength:
		case cEntityEffect::effWaterBreathing:
		case cEntityEffect::effJumpBoost:
		case cEntityEffect::effInvisibility:
		{
			base = 3600;
			break;
		}

		case cEntityEffect::effWeakness:
		case cEntityEffect::effSlowness:
		{
			base = 1800;
			break;
		}
		default: break;
	}

	// If potion is level II, half the duration. If not, stays the same
	TierCoeff = (GetPotionEffectIntensity(a_ItemDamage) > 0) ? 0.5 : 1;

	// If potion is extended, multiply duration by 8 / 3. If not, stays the same
	// Extended potion if sixth lowest bit is set
	ExtCoeff = (a_ItemDamage & 0x40) ? (8.0 / 3.0) : 1;

	// If potion is splash potion, multiply duration by 3 / 4. If not, stays the same
	SplashCoeff = IsPotionDrinkable(a_ItemDamage) ? 1 : 0.75;

	// Ref.:
	//   https://minecraft.gamepedia.com/Java_Edition_data_values#.22Tier.22_bit
	//   https://minecraft.gamepedia.com/Java_Edition_data_values#.22Extended_duration.22_bit
	//   https://minecraft.gamepedia.com/Java_Edition_data_values#.22Splash_potion.22_bit

	return static_cast<int>(base * TierCoeff * ExtCoeff * SplashCoeff);
}





bool cEntityEffect::IsPotionDrinkable(short a_ItemDamage)
{
	// Potions are drinkable if they are not splash potions.
	// i.e. potions are drinkable if the 14th lowest bit is not set
	// Ref.: https://minecraft.gamepedia.com/Java_Edition_data_values#.22Splash_potion.22_bit
	return ((a_ItemDamage & 0x4000) == 0);
}





cEntityEffect::cEntityEffect():
	m_Ticks(0),
	m_Duration(0),
	m_Intensity(0),
	m_DistanceModifier(1)
{

}





cEntityEffect::cEntityEffect(int a_Duration, short a_Intensity, double a_DistanceModifier):
	m_Ticks(0),
	m_Duration(a_Duration),
	m_Intensity(a_Intensity),
	m_DistanceModifier(a_DistanceModifier)
{

}





cEntityEffect::cEntityEffect(const cEntityEffect & a_OtherEffect):
	m_Ticks(a_OtherEffect.m_Ticks),
	m_Duration(a_OtherEffect.m_Duration),
	m_Intensity(a_OtherEffect.m_Intensity),
	m_DistanceModifier(a_OtherEffect.m_DistanceModifier)
{

}





cEntityEffect & cEntityEffect::operator =(cEntityEffect a_OtherEffect)
{
	std::swap(m_Ticks, a_OtherEffect.m_Ticks);
	std::swap(m_Duration, a_OtherEffect.m_Duration);
	std::swap(m_Intensity, a_OtherEffect.m_Intensity);
	std::swap(m_DistanceModifier, a_OtherEffect.m_DistanceModifier);
	return *this;
}





std::unique_ptr<cEntityEffect> cEntityEffect::CreateEntityEffect(cEntityEffect::eType a_EffectType, int a_Duration, short a_Intensity, double a_DistanceModifier)
{
	switch (a_EffectType)
	{
		case cEntityEffect::effNoEffect:       return std::make_unique<cEntityEffect              >(a_Duration, a_Intensity, a_DistanceModifier);

		case cEntityEffect::effAbsorption:     return std::make_unique<cEntityEffectAbsorption    >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effBlindness:      return std::make_unique<cEntityEffectBlindness     >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effFireResistance: return std::make_unique<cEntityEffectFireResistance>(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effHaste:          return std::make_unique<cEntityEffectHaste         >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effHealthBoost:    return std::make_unique<cEntityEffectHealthBoost   >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effHunger:         return std::make_unique<cEntityEffectHunger        >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effInstantDamage:  return std::make_unique<cEntityEffectInstantDamage >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effInstantHealth:  return std::make_unique<cEntityEffectInstantHealth >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effInvisibility:   return std::make_unique<cEntityEffectInvisibility  >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effJumpBoost:      return std::make_unique<cEntityEffectJumpBoost     >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effMiningFatigue:  return std::make_unique<cEntityEffectMiningFatigue >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effNausea:         return std::make_unique<cEntityEffectNausea        >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effNightVision:    return std::make_unique<cEntityEffectNightVision   >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effPoison:         return std::make_unique<cEntityEffectPoison        >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effRegeneration:   return std::make_unique<cEntityEffectRegeneration  >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effResistance:     return std::make_unique<cEntityEffectResistance    >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effSaturation:     return std::make_unique<cEntityEffectSaturation    >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effSlowness:       return std::make_unique<cEntityEffectSlowness      >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effSpeed:          return std::make_unique<cEntityEffectSpeed         >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effStrength:       return std::make_unique<cEntityEffectStrength      >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effWaterBreathing: return std::make_unique<cEntityEffectWaterBreathing>(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effWeakness:       return std::make_unique<cEntityEffectWeakness      >(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effWither:         return std::make_unique<cEntityEffectWither        >(a_Duration, a_Intensity, a_DistanceModifier);
	}
	UNREACHABLE("Unsupported entity effect");
}





void cEntityEffect::OnTick(cPawn & a_Target)
{
	// Reduce the effect's duration
	++m_Ticks;
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectSpeed:

void cEntityEffectSpeed::OnActivate(cPawn & a_Target)
{
	if (a_Target.IsMob())
	{
		cMonster * Mob = static_cast<cMonster*>(&a_Target);
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() + 0.2 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = static_cast<cPlayer*>(&a_Target);
		Player->SetNormalMaxSpeed(Player->GetNormalMaxSpeed() + 0.2 * m_Intensity);
		Player->SetSprintingMaxSpeed(Player->GetSprintingMaxSpeed() + 0.26 * m_Intensity);
		Player->SetFlyingMaxSpeed(Player->GetFlyingMaxSpeed() + 0.2 * m_Intensity);
	}
}





void cEntityEffectSpeed::OnDeactivate(cPawn & a_Target)
{
	if (a_Target.IsMob())
	{
		cMonster * Mob = static_cast<cMonster*>(&a_Target);
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() - 0.2 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = static_cast<cPlayer*>(&a_Target);
		Player->SetNormalMaxSpeed(Player->GetNormalMaxSpeed() - 0.2 * m_Intensity);
		Player->SetSprintingMaxSpeed(Player->GetSprintingMaxSpeed() - 0.26 * m_Intensity);
		Player->SetFlyingMaxSpeed(Player->GetFlyingMaxSpeed() - 0.2 * m_Intensity);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectSlowness:

void cEntityEffectSlowness::OnActivate(cPawn & a_Target)
{
	if (a_Target.IsMob())
	{
		cMonster * Mob = static_cast<cMonster*>(&a_Target);
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() - 0.15 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = static_cast<cPlayer*>(&a_Target);
		Player->SetNormalMaxSpeed(Player->GetNormalMaxSpeed() - 0.15 * m_Intensity);
		Player->SetSprintingMaxSpeed(Player->GetSprintingMaxSpeed() - 0.195 * m_Intensity);
		Player->SetFlyingMaxSpeed(Player->GetFlyingMaxSpeed() - 0.15 * m_Intensity);
	}
}





void cEntityEffectSlowness::OnDeactivate(cPawn & a_Target)
{
	if (a_Target.IsMob())
	{
		cMonster * Mob = static_cast<cMonster*>(&a_Target);
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() + 0.15 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = static_cast<cPlayer*>(&a_Target);
		Player->SetNormalMaxSpeed(Player->GetNormalMaxSpeed() + 0.15 * m_Intensity);
		Player->SetSprintingMaxSpeed(Player->GetSprintingMaxSpeed() + 0.195 * m_Intensity);
		Player->SetFlyingMaxSpeed(Player->GetFlyingMaxSpeed() + 0.15 * m_Intensity);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectInstantHealth:

void cEntityEffectInstantHealth::OnActivate(cPawn & a_Target)
{
	// Base amount = 6, doubles for every increase in intensity
	int amount = static_cast<int>(6 * (1 << m_Intensity) * m_DistanceModifier);

	if (a_Target.IsMob() && static_cast<cMonster &>(a_Target).IsUndead())
	{
		a_Target.TakeDamage(dtPotionOfHarming, nullptr, amount, 0);  // TODO: Store attacker in a pointer-safe way, pass to TakeDamage
		return;
	}
	a_Target.Heal(amount);
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectInstantDamage:

void cEntityEffectInstantDamage::OnActivate(cPawn & a_Target)
{
	// Base amount = 6, doubles for every increase in intensity
	int amount = static_cast<int>(6 * (1 << m_Intensity) * m_DistanceModifier);

	if (a_Target.IsMob() && static_cast<cMonster &>(a_Target).IsUndead())
	{
		a_Target.Heal(amount);
		return;
	}
	a_Target.TakeDamage(dtPotionOfHarming, nullptr, amount, 0);  // TODO: Store attacker in a pointer-safe way, pass to TakeDamage
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectRegeneration:

void cEntityEffectRegeneration::OnTick(cPawn & a_Target)
{
	Super::OnTick(a_Target);

	if (a_Target.IsMob() && static_cast<cMonster &>(a_Target).IsUndead())
	{
		return;
	}

	// Regen frequency = 50 ticks, divided by potion level (Regen II = 25 ticks)
	int frequency = std::max(1, FloorC(50.0 / static_cast<double>(m_Intensity + 1)));

	if ((m_Ticks % frequency) != 0)
	{
		return;
	}

	a_Target.Heal(1);
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectHunger:

void cEntityEffectHunger::OnTick(cPawn & a_Target)
{
	Super::OnTick(a_Target);

	if (a_Target.IsPlayer())
	{
		cPlayer & Target = static_cast<cPlayer &>(a_Target);
		Target.AddFoodExhaustion(0.025 * (static_cast<double>(GetIntensity()) + 1.0));  // 0.5 per second = 0.025 per tick
	}
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectInvisibility:

void cEntityEffectInvisibility::BroadcastMetadata(cPawn & a_Target)
{
	auto World = a_Target.GetWorld();
	if (World != nullptr)
	{
		World->BroadcastEntityMetadata(a_Target);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectWeakness:

void cEntityEffectWeakness::OnTick(cPawn & a_Target)
{
	Super::OnTick(a_Target);

	// Damage reduction = 0.5 damage, multiplied by potion level (Weakness II = 1 damage)
	// double dmg_reduc = 0.5 * (a_Effect.GetIntensity() + 1);

	// TODO: Implement me!
	// TODO: Weakened villager zombies can be turned back to villagers with the god apple
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectPoison:

void cEntityEffectPoison::OnTick(cPawn & a_Target)
{
	Super::OnTick(a_Target);

	if (a_Target.IsMob())
	{
		cMonster & Target = static_cast<cMonster &>(a_Target);

		// Doesn't effect undead mobs, spiders
		if (
			Target.IsUndead() ||
			(Target.GetMobType() == mtSpider) ||
			(Target.GetMobType() == mtCaveSpider)
		)
		{
			return;
		}
	}

	// Poison frequency = 25 ticks, divided by potion level (Poison II = 12 ticks)
	int frequency = std::max(1, FloorC(25.0 / static_cast<double>(m_Intensity + 1)));

	if ((m_Ticks % frequency) == 0)
	{
		// Cannot take poison damage when health is at 1
		if (a_Target.GetHealth() > 1)
		{
			a_Target.TakeDamage(dtPoisoning, nullptr, 1, 0);
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectWither:

void cEntityEffectWither::OnTick(cPawn & a_Target)
{
	Super::OnTick(a_Target);

	// Damage frequency = 40 ticks, divided by effect level (Wither II = 20 ticks)
	int frequency = std::max(1, FloorC(40.0 / static_cast<double>(m_Intensity + 1)));

	if ((m_Ticks % frequency) == 0)
	{
		a_Target.TakeDamage(dtWither, nullptr, 1, 0);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectSaturation:

void cEntityEffectSaturation::OnTick(cPawn & a_Target)
{
	if (a_Target.IsPlayer())
	{
		cPlayer & Target = static_cast<cPlayer &>(a_Target);
		Target.SetFoodSaturationLevel(Target.GetFoodSaturationLevel() + (1 + m_Intensity));  // Increase saturation 1 per tick, adds 1 for every increase in level
	}
}
