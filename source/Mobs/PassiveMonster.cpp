
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveMonster.h"
#include "../MersenneTwister.h"





cPassiveMonster::cPassiveMonster(const AString & a_ConfigName, char a_ProtocolMobType, const AString & a_SoundHurt, const AString & a_SoundDeath) :
	super(a_ConfigName, a_ProtocolMobType, a_SoundHurt, a_SoundDeath)
{
	m_EMPersonality = PASSIVE;
}





void cPassiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if ((a_TDI.Attacker != this) && (a_TDI.Attacker != NULL))
	{
		m_EMState = ESCAPING;
	}
}





void cPassiveMonster::Tick(float a_Dt)
{
	super::Tick(a_Dt);

	m_SeePlayerInterval += a_Dt;

	if (m_SeePlayerInterval > 1)  // Check every second
	{
		MTRand r1;
		int rem = r1.randInt() % 3 + 1;  // Check most of the time but miss occasionally

		m_SeePlayerInterval = 0.0;
		if (rem >= 2)
		{
			if (m_EMState == ESCAPING)
			{
				CheckEventLostPlayer();
			} 
		}
	}
}




