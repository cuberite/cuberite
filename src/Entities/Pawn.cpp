
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"





cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height)
	: cEntity(a_EntityType, 0, 0, 0, a_Width, a_Height)
	, m_bBurnable(true)
	, m_EntityEffects(std::map<cEntityEffect::eType, cEntityEffect>())
{
}





void cPawn::Tick(float a_Dt, cChunk & a_Chunk)
{
	
	
	super::Tick(a_Dt, a_Chunk);
}





void cPawn::AddEntityEffect(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect)
{
	m_EntityEffects[a_EffectType] = a_Effect;
}





void cPawn::RemoveEntityEffect(cEntityEffect::eType a_EffectType)
{
	m_EntityEffects.erase(a_EffectType);
}
