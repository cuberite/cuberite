
#pragma once

#include "PassiveAggressiveMonster.h"





class cEnderman :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cEnderman(void);

	CLASS_PROTODEF(cEnderman);

	virtual void Tick(float a_Dt, MTRand & a_TickRandom) override;
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




