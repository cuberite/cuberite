
#pragma once

#include "AggressiveMonster.h"





class cGhast :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cGhast(void);

	virtual bool IsA(const char * a_EntityType) override;

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




