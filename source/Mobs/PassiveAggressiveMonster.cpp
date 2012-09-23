
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveAggressiveMonster.h"

#include "../cPlayer.h"





cPassiveAggressiveMonster::cPassiveAggressiveMonster()
{
	m_EMPersonality = PASSIVE;
}

cPassiveAggressiveMonster::~cPassiveAggressiveMonster()
{
}

void cPassiveAggressiveMonster::TakeDamage(int a_Damage, cEntity* a_Instigator)
{
	cMonster::TakeDamage(a_Damage, a_Instigator);
	if(m_Target->GetEntityType() == cEntity::eEntityType_Player)
	{
		cPlayer * Player = (cPlayer *) m_Target;
		if(Player->GetGameMode() != 1)
		{
			m_EMState = CHASING;
		}
	}
	
}

void cPassiveAggressiveMonster::EventSeePlayer(cEntity *a_Entity)
{
	return cMonster::EventSeePlayer(a_Entity);
}