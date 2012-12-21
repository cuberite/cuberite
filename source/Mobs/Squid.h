
#pragma once

#include "PassiveMonster.h"





class cSquid :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cSquid();

	virtual void Tick(float a_Dt) override;

	virtual bool IsA(const char * a_EntityType) override;
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




