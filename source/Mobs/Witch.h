
#pragma once

#include "AggressiveMonster.h"





class cWitch :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cWitch();

	CLASS_PROTODEF(cWitch);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




