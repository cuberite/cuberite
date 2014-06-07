
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"





cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height)
	: cEntity(a_EntityType, 0, 0, 0, a_Width, a_Height)
	, m_EntityEffects(std::map<cEntityEffect::eType, cEntityEffect>())
{
}





void cPawn::Tick(float a_Dt, cChunk & a_Chunk)
{
	// Iterate through this entity's applied effects
	for (std::map<cEntityEffect::eType, cEntityEffect>::iterator iter = m_EntityEffects.begin();
		 iter != m_EntityEffects.end();
		 ++iter)
	{
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
	//m_World->BroadcastEntityEffect(*this, a_EffectType, a_Effect.m_Intensity, a_Effect.m_Ticks);
}





void cPawn::RemoveEntityEffect(cEntityEffect::eType a_EffectType)
{
	m_EntityEffects.erase(a_EffectType);
	//m_World->BroadcastRemoveEntityEffect(*this, a_EffectType);
}
