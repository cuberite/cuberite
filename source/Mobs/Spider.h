
#pragma once

#include "AggressiveMonster.h"





class cSpider :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSpider(void);

	CLASS_PROTODEF(cSpider);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




