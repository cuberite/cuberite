#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EntityEffects.h"





cEntityEffect::cEntityEffect():
	m_Ticks(0),
	m_Intensity(0)
{
	
}





cEntityEffect::cEntityEffect(int a_Ticks, short a_Intensity):
	m_Ticks(a_Ticks),
	m_Intensity(a_Intensity)
{
	
}
