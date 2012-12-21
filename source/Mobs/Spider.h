
#pragma once

#include "AggressiveMonster.h"





class cSpider :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSpider();

	virtual bool IsA(const char * a_EntityType) override;

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




