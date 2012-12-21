
#pragma once

#include "PassiveAggressiveMonster.h"





class cEnderman :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cEnderman(void);

	virtual bool IsA(const char * a_EntityType) override;

	virtual void Tick(float a_Dt) override;
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




