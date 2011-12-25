#pragma once

#include "cMonster.h"

class cPassiveMonster : public cMonster
{
public:
	cPassiveMonster();
	~cPassiveMonster();

	virtual void Tick(float a_Dt);

	virtual void TakeDamage(int a_Damage, cEntity* a_Instigator);
};
