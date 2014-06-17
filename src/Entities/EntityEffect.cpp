#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EntityEffect.h"
#include "../Mobs/Monster.h"
#include "Player.h"




cEntityEffect::cEntityEffect():
	m_Ticks(0),
	m_Duration(0),
	m_Intensity(0),
	m_Creator(NULL),
	m_DistanceModifier(1)
{
	
}





cEntityEffect::cEntityEffect(int a_Duration, short a_Intensity, cPawn *a_Creator, double a_DistanceModifier):
	m_Ticks(0),
	m_Duration(a_Duration),
	m_Intensity(a_Intensity),
	m_Creator(a_Creator),
	m_DistanceModifier(a_DistanceModifier)
{
	
}





cEntityEffect::~cEntityEffect()
{
	
}





cEntityEffect * cEntityEffect::CreateEntityEffect(cEntityEffect::eType a_EffectType, int a_Duration, short a_Intensity, cPawn * a_Creator, double a_DistanceModifier)
{
	switch (a_EffectType)
	{
		case cEntityEffect::effNoEffect:       return new cEntityEffect              (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		
		case cEntityEffect::effAbsorption:     return new cEntityEffectAbsorption    (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effBlindness:      return new cEntityEffectBlindness     (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effFireResistance: return new cEntityEffectFireResistance(a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effHaste:          return new cEntityEffectHaste         (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effHealthBoost:    return new cEntityEffectHealthBoost   (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effHunger:         return new cEntityEffectHunger        (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effInstantDamage:  return new cEntityEffectInstantDamage (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effInstantHealth:  return new cEntityEffectInstantHealth (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effInvisibility:   return new cEntityEffectInvisibility  (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effJumpBoost:      return new cEntityEffectJumpBoost     (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effMiningFatigue:  return new cEntityEffectMiningFatigue (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effNausea:         return new cEntityEffectNausea        (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effNightVision:    return new cEntityEffectNightVision   (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effPoison:         return new cEntityEffectPoison        (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effRegeneration:   return new cEntityEffectRegeneration  (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effResistance:     return new cEntityEffectResistance    (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effSaturation:     return new cEntityEffectSaturation    (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effSlowness:       return new cEntityEffectSlowness      (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effSpeed:          return new cEntityEffectSpeed         (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effStrength:       return new cEntityEffectStrength      (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effWaterBreathing: return new cEntityEffectWaterBreathing(a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effWeakness:       return new cEntityEffectWeakness      (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
		case cEntityEffect::effWither:         return new cEntityEffectWither        (a_Duration, a_Intensity, a_Creator, a_DistanceModifier);
	}
	
	ASSERT(!"Unhandled entity effect type!");
}





void cEntityEffect::OnTick(cPawn & a_Target)
{
	// Reduce the effect's duration
	++m_Ticks;
}





void cEntityEffect::OnActivate(cPawn & a_Target)
{
}





void cEntityEffect::OnDeactivate(cPawn & a_Target)
{
}





/************************************************************************
 **** Instant Health
 ************************************************************************/
void cEntityEffectInstantHealth::OnActivate(cPawn & a_Target)
{
	// Base amount = 6, doubles for every increase in intensity
	int amount = (int)(6 * std::pow(2.0, m_Intensity) * m_DistanceModifier);
	
	if (a_Target.IsMob())
	{
		if (((cMonster &) a_Target).IsUndead())
		{
			a_Target.TakeDamage(dtPotionOfHarming, m_Creator, amount, 0);
			return;
		}
	}
	a_Target.Heal(amount);
}





/************************************************************************
 **** Instant Damage
 ************************************************************************/
void cEntityEffectInstantDamage::OnActivate(cPawn & a_Target)
{
	// Base amount = 6, doubles for every increase in intensity
	int amount = (int)(6 * std::pow(2.0, m_Intensity) * m_DistanceModifier);
	
	if (a_Target.IsMob())
	{
		if (((cMonster &) a_Target).IsUndead())
		{
			a_Target.Heal(amount);
			return;
		}
	}
	a_Target.TakeDamage(dtPotionOfHarming, m_Creator, amount, 0);
}





/************************************************************************
 **** Regeneration
 ************************************************************************/
void cEntityEffectRegeneration::OnTick(cPawn & a_Target)
{
	super::OnTick(a_Target);

	if (a_Target.IsMob())
	{
		if (((cMonster &) a_Target).IsUndead())
		{
			return;
		}
	}
	
	// Regen frequency = 50 ticks, divided by potion level (Regen II = 25 ticks)
	int frequency = (int) std::floor(50.0 / (double)(m_Intensity + 1));
	
	if (m_Ticks % frequency != 0)
	{
		return;
	}

	a_Target.Heal(1);
}





/************************************************************************
 **** Hunger
 ************************************************************************/
void cEntityEffectHunger::OnTick(cPawn & a_Target)
{
	super::OnTick(a_Target);
	
	if (a_Target.IsPlayer())
	{
		cPlayer & Target = (cPlayer &) a_Target;
		Target.SetFoodExhaustionLevel(Target.GetFoodExhaustionLevel() + 0.025); // 0.5 per second = 0.025 per tick
	}
}





/************************************************************************
 **** Weakness
 ************************************************************************/
void cEntityEffectWeakness::OnTick(cPawn & a_Target)
{
	super::OnTick(a_Target);
	
	// Damage reduction = 0.5 damage, multiplied by potion level (Weakness II = 1 damage)
	// double dmg_reduc = 0.5 * (a_Effect.GetIntensity() + 1);
	
	// TODO: Implement me!
	// TODO: Weakened villager zombies can be turned back to villagers with the god apple
}





/************************************************************************
 **** Poison
 ************************************************************************/
void cEntityEffectPoison::OnTick(cPawn & a_Target)
{
	super::OnTick(a_Target);
	
	if (a_Target.IsMob())
	{
		cMonster & Target = (cMonster &) a_Target;
		
		// Doesn't effect undead mobs, spiders
		if (Target.IsUndead()
			|| Target.GetMobType() == cMonster::mtSpider
			|| Target.GetMobType() == cMonster::mtCaveSpider)
		{
			return;
		}
	}
	
	// Poison frequency = 25 ticks, divided by potion level (Poison II = 12 ticks)
	int frequency = (int) std::floor(25.0 / (double)(m_Intensity + 1));
	
	if (m_Ticks % frequency == 0)
	{
		// Cannot take poison damage when health is at 1
		if (a_Target.GetHealth() > 1)
		{
			a_Target.TakeDamage(dtPoisoning, m_Creator, 1, 0);
		}
	}
}





/************************************************************************
 **** Wither
 ************************************************************************/
void cEntityEffectWither::OnTick(cPawn & a_Target)
{
	super::OnTick(a_Target);
	
	// Poison frequency = 40 ticks, divided by effect level (Wither II = 20 ticks)
	int frequency = (int) std::floor(25.0 / (double)(m_Intensity + 1));

	if (m_Ticks % frequency == 0)
	{
		a_Target.TakeDamage(dtWither, m_Creator, 1, 0);
	}
	//TODO: "<Player> withered away>
}





/************************************************************************
 **** Saturation
 ************************************************************************/
void cEntityEffectSaturation::OnTick(cPawn & a_Target)
{
	if (a_Target.IsPlayer())
	{
		cPlayer & Target = (cPlayer &) a_Target;
		Target.SetFoodSaturationLevel(Target.GetFoodSaturationLevel() + (1 + m_Intensity)); // Increase saturation 1 per tick, adds 1 for every increase in level
	}
}
