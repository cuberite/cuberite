#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EntityEffects.h"
#include "Pawn.h"




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
