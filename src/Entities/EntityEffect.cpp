#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EntityEffect.h"
#include "../Mobs/Monster.h"
#include "Player.h"





int cEntityEffect::GetPotionColor(short a_ItemDamage)
{
	// Lowest six bits
	return (a_ItemDamage & 0x3f);
}





cEntityEffect::eType cEntityEffect::GetPotionEffectType(short a_ItemDamage)
{
	// Lowest four bits
	// Potion effect bits are different from entity effect values
	// For reference: http://minecraft.gamepedia.com/Data_values#.22Potion_effect.22_bits
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
	//   http://minecraft.gamepedia.com/Data_values#.22Tier.22_bit
	//   http://minecraft.gamepedia.com/Data_values#.22Extended_duration.22_bit
	//   http://minecraft.gamepedia.com/Data_values#.22Splash_potion.22_bit
	
	return (int)(base * TierCoeff * ExtCoeff * SplashCoeff);
}





bool cEntityEffect::IsPotionDrinkable(short a_ItemDamage)
{
	// Drinkable potion if 13th lowest bit is set
	// Ref.: http://minecraft.gamepedia.com/Potions#Data_value_table
	return ((a_ItemDamage & 0x2000) != 0);
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





cEntityEffect * cEntityEffect::CreateEntityEffect(cEntityEffect::eType a_EffectType, int a_Duration, short a_Intensity, double a_DistanceModifier)
{
	switch (a_EffectType)
	{
		case cEntityEffect::effNoEffect:       return new cEntityEffect              (a_Duration, a_Intensity, a_DistanceModifier);
		
		case cEntityEffect::effAbsorption:     return new cEntityEffectAbsorption    (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effBlindness:      return new cEntityEffectBlindness     (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effFireResistance: return new cEntityEffectFireResistance(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effHaste:          return new cEntityEffectHaste         (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effHealthBoost:    return new cEntityEffectHealthBoost   (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effHunger:         return new cEntityEffectHunger        (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effInstantDamage:  return new cEntityEffectInstantDamage (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effInstantHealth:  return new cEntityEffectInstantHealth (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effInvisibility:   return new cEntityEffectInvisibility  (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effJumpBoost:      return new cEntityEffectJumpBoost     (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effMiningFatigue:  return new cEntityEffectMiningFatigue (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effNausea:         return new cEntityEffectNausea        (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effNightVision:    return new cEntityEffectNightVision   (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effPoison:         return new cEntityEffectPoison        (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effRegeneration:   return new cEntityEffectRegeneration  (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effResistance:     return new cEntityEffectResistance    (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effSaturation:     return new cEntityEffectSaturation    (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effSlowness:       return new cEntityEffectSlowness      (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effSpeed:          return new cEntityEffectSpeed         (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effStrength:       return new cEntityEffectStrength      (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effWaterBreathing: return new cEntityEffectWaterBreathing(a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effWeakness:       return new cEntityEffectWeakness      (a_Duration, a_Intensity, a_DistanceModifier);
		case cEntityEffect::effWither:         return new cEntityEffectWither        (a_Duration, a_Intensity, a_DistanceModifier);
	}
	
	ASSERT(!"Unhandled entity effect type!");
	return nullptr;
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
		cMonster * Mob = (cMonster*) &a_Target;
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() + 0.2 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = (cPlayer*) &a_Target;
		Player->SetNormalMaxSpeed(Player->GetNormalMaxSpeed() + 0.2 * m_Intensity);
		Player->SetSprintingMaxSpeed(Player->GetSprintingMaxSpeed() + 0.26 * m_Intensity);
		Player->SetFlyingMaxSpeed(Player->GetFlyingMaxSpeed() + 0.2 * m_Intensity);
	}
}





void cEntityEffectSpeed::OnDeactivate(cPawn & a_Target)
{
	if (a_Target.IsMob())
	{
		cMonster * Mob = (cMonster*) &a_Target;
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() - 0.2 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = (cPlayer*) &a_Target;
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
		cMonster * Mob = (cMonster*) &a_Target;
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() - 0.15 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = (cPlayer*) &a_Target;
		Player->SetNormalMaxSpeed(Player->GetNormalMaxSpeed() - 0.15 * m_Intensity);
		Player->SetSprintingMaxSpeed(Player->GetSprintingMaxSpeed() - 0.195 * m_Intensity);
		Player->SetFlyingMaxSpeed(Player->GetFlyingMaxSpeed() - 0.15 * m_Intensity);
	}
}





void cEntityEffectSlowness::OnDeactivate(cPawn & a_Target)
{
	if (a_Target.IsMob())
	{
		cMonster * Mob = (cMonster*) &a_Target;
		Mob->SetRelativeWalkSpeed(Mob->GetRelativeWalkSpeed() + 0.15 * m_Intensity);
	}
	else if (a_Target.IsPlayer())
	{
		cPlayer * Player = (cPlayer*) &a_Target;
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
	int amount = (int)(6 * (1 << m_Intensity) * m_DistanceModifier);
	
	if (a_Target.IsMob() && ((cMonster &) a_Target).IsUndead())
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
	int amount = (int)(6 * (1 << m_Intensity) * m_DistanceModifier);
	
	if (a_Target.IsMob() && ((cMonster &) a_Target).IsUndead())
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
	super::OnTick(a_Target);

	if (a_Target.IsMob() && ((cMonster &) a_Target).IsUndead())
	{
		return;
	}
	
	// Regen frequency = 50 ticks, divided by potion level (Regen II = 25 ticks)
	int frequency = (int) std::floor(50.0 / (double)(m_Intensity + 1));
	
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
	super::OnTick(a_Target);
	
	if (a_Target.IsPlayer())
	{
		cPlayer & Target = (cPlayer &) a_Target;
		Target.AddFoodExhaustion(0.025 * ((double)GetIntensity() + 1.0));  // 0.5 per second = 0.025 per tick
	}
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectWeakness:

void cEntityEffectWeakness::OnTick(cPawn & a_Target)
{
	super::OnTick(a_Target);
	
	// Damage reduction = 0.5 damage, multiplied by potion level (Weakness II = 1 damage)
	// double dmg_reduc = 0.5 * (a_Effect.GetIntensity() + 1);
	
	// TODO: Implement me!
	// TODO: Weakened villager zombies can be turned back to villagers with the god apple
}





////////////////////////////////////////////////////////////////////////////////
// cEntityEffectPoison:

void cEntityEffectPoison::OnTick(cPawn & a_Target)
{
	super::OnTick(a_Target);
	
	if (a_Target.IsMob())
	{
		cMonster & Target = (cMonster &) a_Target;
		
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
	int frequency = (int) std::floor(25.0 / (double)(m_Intensity + 1));
	
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
	super::OnTick(a_Target);
	
	// Damage frequency = 40 ticks, divided by effect level (Wither II = 20 ticks)
	int frequency = (int) std::floor(25.0 / (double)(m_Intensity + 1));

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
		cPlayer & Target = (cPlayer &) a_Target;
		Target.SetFoodSaturationLevel(Target.GetFoodSaturationLevel() + (1 + m_Intensity));  // Increase saturation 1 per tick, adds 1 for every increase in level
	}
}




