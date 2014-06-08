
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "../World.h"





cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height)
	: cEntity(a_EntityType, 0, 0, 0, a_Width, a_Height)
	, m_EntityEffects(tEffectMap())
{
}





void cPawn::Tick(float a_Dt, cChunk & a_Chunk)
{
	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin(); iter != m_EntityEffects.end();)
	{
		// Copies values to prevent pesky wrong accesses and erasures
		cEntityEffect::eType effect_type = iter->first;
		cEntityEffect &effect_values = iter->second;
		
		// Apply entity effect
		HandleEntityEffects(effect_type, effect_values);
		
		// Reduce the effect's duration
		effect_values.m_Ticks--;
		
		// Iterates (must be called before any possible erasure)
		++iter;
		
		// Remove effect if duration has elapsed
		if (effect_values.m_Ticks <= 0)
		{
			RemoveEntityEffect(effect_type);
		}
		
		// TODO: Check for discrepancies between client and server effect values
	}
	
	super::Tick(a_Dt, a_Chunk);
}





void cPawn::KilledBy(cEntity *a_Killer)
{
	ClearEntityEffects();
}





void cPawn::AddEntityEffect(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect)
{
	if (a_EffectType == cEntityEffect::effNoEffect)
	{
		return;
	}
	
	m_EntityEffects[a_EffectType] = a_Effect;
	m_World->BroadcastEntityEffect(*this, a_EffectType, a_Effect.GetIntensity(),
								   a_Effect.m_Ticks * a_Effect.GetDistanceModifier());
}





void cPawn::RemoveEntityEffect(cEntityEffect::eType a_EffectType)
{
	m_EntityEffects.erase(a_EffectType);
	m_World->BroadcastRemoveEntityEffect(*this, a_EffectType);
}





void cPawn::ClearEntityEffects()
{
	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin(); iter != m_EntityEffects.end();)
	{
		// Copy values to prevent pesky wrong erasures
		cEntityEffect::eType effect_type = iter->first;
		
		// Iterates (must be called before any possible erasure)
		++iter;
		
		// Remove effect
		RemoveEntityEffect(effect_type);
	}
}





void cPawn::HandleEntityEffects(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect)
{
	switch (a_EffectType)
	{
		// Default effect behaviors
		case cEntityEffect::effInstantHealth:
		{
			// Base heal = 6, doubles for every increase in intensity
			Heal(6 * std::pow(2, a_Effect.GetIntensity()) * a_Effect.GetDistanceModifier());
			
			// TODO: Harms undead
			return;
		}
		case cEntityEffect::effInstantDamage:
		{
			// Base damage = 6, doubles for every increase in intensity
			int damage = 6 * std::pow(2, a_Effect.GetIntensity());
			TakeDamage(dtPotionOfHarming, a_Effect.GetUser(), damage * a_Effect.GetDistanceModifier(), 0);
			
			// TODO: Heals undead
			return;
		}
		case cEntityEffect::effStrength:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::effWeakness:
		{
			// Damage reduction = 0.5 damage, multiplied by potion level (Weakness II = 1 damage)
			//double dmg_reduc = 0.5 * (a_Effect.GetIntensity() + 1);
			
			// TODO: Implement me!
			// TODO: Weakened villager zombies can be turned back to villagers with the god apple
			return;
		}
		case cEntityEffect::effRegeneration:
		{
			// Regen frequency = 50 ticks, divided by potion level (Regen II = 25 ticks)
			int frequency = std::floor(50.0 / (double)(a_Effect.GetIntensity() + 1));
			
			static short counter = 0;
			if (++counter >= frequency)
			{
				Heal(1);
				counter = 0;
			}
			
			// TODO: Doesn't effect undead
			return;
		}
		case cEntityEffect::effPoison:
		{
			// Poison frequency = 25 ticks, divided by potion level (Poison II = 25 ticks)
			int frequency = std::floor(25.0 / (double)(a_Effect.GetIntensity() + 1));
			
			static short counter = 0;
			if (++counter >= frequency)
			{
				// Cannot take poison damage when health is at 1
				if (GetHealth() > 1)
				{
					TakeDamage(dtPoisoning, a_Effect.GetUser(), 1, 0);
				}
				counter = 0;
			}
			
			// TODO: Doesn't effect undead or spiders
			return;
		}
		case cEntityEffect::effFireResistance:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::effSpeed:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::effSlowness:
		{
			// TODO: Implement me!
			return;
		}
	}
}
