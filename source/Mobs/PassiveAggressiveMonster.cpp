
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveAggressiveMonster.h"

#include "../Player.h"





cPassiveAggressiveMonster::cPassiveAggressiveMonster(void)
{
	m_EMPersonality = PASSIVE;
}





void cPassiveAggressiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	
	if ((m_Target != NULL) && (m_Target->IsPlayer()))
	{
		cPlayer * Player = (cPlayer *) m_Target;
		if (Player->GetGameMode() != 1)
		{
			m_EMState = CHASING;
		}
	}
}




