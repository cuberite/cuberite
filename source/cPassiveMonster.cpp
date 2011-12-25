#include "cPassiveMonster.h"


cPassiveMonster::cPassiveMonster()
{
	m_EMPersonality = PASSIVE;
}

cPassiveMonster::~cPassiveMonster()
{
}

void cPassiveMonster::TakeDamage(int a_Damage, cEntity* a_Instigator)
{
	cMonster::TakeDamage(a_Damage, a_Instigator);
	m_EMState = ESCAPING;
}

void cPassiveMonster::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	m_SeePlayerInterval += a_Dt;

	if(m_SeePlayerInterval > 1)
	{
		int rem = rand() % 3 + 1; //check most of the time but miss occasionally

		m_SeePlayerInterval = 0.0;
		if(rem >= 2) {
			if(m_EMState == ESCAPING)
			{
				CheckEventLostPlayer();
			} 
		}
	}
}