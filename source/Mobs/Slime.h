
#pragma once

#include "AggressiveMonster.h"

class cSlime :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSlime();

	virtual bool IsA(const char * a_EntityType) override;

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




