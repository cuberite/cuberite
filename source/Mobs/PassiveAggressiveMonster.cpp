
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveAggressiveMonster.h"

#include "../Entities/Player.h"





cPassiveAggressiveMonster::cPassiveAggressiveMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height)
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




