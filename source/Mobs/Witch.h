
#pragma once

#include "AggressiveMonster.h"





class cWitch :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cWitch();

	virtual bool IsA(const char* a_EntityType) override;

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




