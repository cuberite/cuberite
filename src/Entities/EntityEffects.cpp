#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EntityEffects.h"
#include "Pawn.h"




cEntityEffect::cEntityEffect():
	m_Ticks(0),
	m_Intensity(0),
	m_User(NULL),
	m_DistanceModifier(1)
{
	
}





cEntityEffect::cEntityEffect(int a_Ticks, short a_Intensity, cPawn *a_User, double a_DistanceModifier):
	m_Ticks(a_Ticks),
	m_Intensity(a_Intensity),
	m_User(a_User),
	m_DistanceModifier(a_DistanceModifier)
{
	
}