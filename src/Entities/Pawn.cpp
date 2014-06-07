
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"
#include "../World.h"





cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height)
	: cEntity(a_EntityType, 0, 0, 0, a_Width, a_Height)
	, m_EntityEffects(std::map<cEntityEffect::eType, cEntityEffect>())
{
}





void cPawn::Tick(float a_Dt, cChunk & a_Chunk)
{
	// Iterate through this entity's applied effects
	for (tEffectMap::iterator iter = m_EntityEffects.begin();
		 iter != m_EntityEffects.end();
		 ++iter)
	{
		// Apply entity effect
		HandleEntityEffects(iter->first, iter->second);
		
		// Reduce the effect's duration
		iter->second.m_Ticks--;
		
		// Remove effect if duration has elapsed
		if (iter->second.m_Ticks <= 0)
		{
			RemoveEntityEffect(iter->first);
		}
		
		// TODO: Check for discrepancies between client and server effect values
	}
	
	super::Tick(a_Dt, a_Chunk);
}





void cPawn::AddEntityEffect(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect)
{
	m_EntityEffects[a_EffectType] = a_Effect;
	m_World->BroadcastEntityEffect(*this, a_EffectType, a_Effect.GetIntensity(), a_Effect.m_Ticks);
}





void cPawn::RemoveEntityEffect(cEntityEffect::eType a_EffectType)
{
	m_EntityEffects.erase(a_EffectType);
	m_World->BroadcastRemoveEntityEffect(*this, a_EffectType);
}





void cPawn::HandleEntityEffects(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect)
{
	switch (a_EffectType)
	{
		// Default effect behaviors
		case cEntityEffect::efInstantHealth:
		{
			// Base heal = 6, doubles for every increase in intensity
			Heal(6 * std::pow(2, a_Effect.GetIntensity()));
			
			// TODO: Harms undead
			return;
		}
		case cEntityEffect::efInstantDamage:
		{
			// Base damage = 6, doubles for every increase in intensity
			int damage = 6 * std::pow(2, a_Effect.GetIntensity());
			TakeDamage(dtPotionOfHarming, a_Effect.GetUser(), damage, 0);
			
			// TODO: Heals undead
			return;
		}
		case cEntityEffect::efStrength:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::efWeakness:
		{
			// Damage reduction = 0.5 damage, multiplied by potion level (Weakness II = 1 damage)
			//double dmg_reduc = 0.5 * (a_Effect.GetIntensity() + 1);
			
			// TODO: Implement me!
			// TODO: Weakened villager zombies can be turned back to villagers with the god apple
			return;
		}
		case cEntityEffect::efRegeneration:
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
		case cEntityEffect::efPoison:
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
		case cEntityEffect::efFireResistance:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::efSpeed:
		{
			// TODO: Implement me!
			return;
		}
		case cEntityEffect::efSlowness:
		{
			// TODO: Implement me!
			return;
		}
	}
}
