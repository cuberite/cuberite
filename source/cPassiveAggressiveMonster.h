#pragma once

#include "cAggressiveMonster.h"

class cPassiveAggressiveMonster : public cAggressiveMonster
{
public:
	cPassiveAggressiveMonster();
	~cPassiveAggressiveMonster();

	virtual void TakeDamage(int a_Damage, cEntity* a_Instigator);
	void EventSeePlayer(cEntity *a_Entity);
};
