
#pragma once

#include "PassiveMonster.h"





class cSquid :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cSquid();

	virtual void Tick(float a_Dt, MTRand & a_TickRandom) override;

	CLASS_PROTODEF(cSquid);
	
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




