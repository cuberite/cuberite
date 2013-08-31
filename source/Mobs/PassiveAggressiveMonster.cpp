#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveAggressiveMonster.h"

#include "../World.h"
#include "../Vector3f.h"
#include "../Entities/Player.h"
#include "../MersenneTwister.h"





cPassiveAggressiveMonster::cPassiveAggressiveMonster()
{
	m_EMPersonality = PASSIVE;
}








void cPassiveAggressiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if ((a_TDI.Attacker != NULL) && (a_TDI.Attacker->IsPlayer()))
	{
		cPlayer * Player = (cPlayer *) a_TDI.Attacker;
		if (Player->GetGameMode() != 1)
		{
			m_EMState = CHASING;
		}
	}
}




