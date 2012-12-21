
#pragma once

#include "PassiveAggressiveMonster.h"





class cZombiepigman :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cZombiepigman(void);

	CLASS_PROTODEF(cZombiepigman);

	virtual void Tick(float a_Dt) override;
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
	virtual void KilledBy(cPawn * a_Killer) override;
} ;




