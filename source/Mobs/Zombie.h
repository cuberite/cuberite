#pragma once

#include "AggressiveMonster.h"





class cZombie :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cZombie();

	CLASS_PROTODEF(cZombie);
	
	virtual void Tick(float a_Dt, MTRand & a_TickRandom) override;
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




