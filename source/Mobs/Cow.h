
#pragma once

#include "PassiveMonster.h"





class cCow :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cCow();

	virtual bool IsA(const char * a_EntityType) override;

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




