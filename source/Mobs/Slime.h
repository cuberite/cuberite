
#pragma once

#include "AggressiveMonster.h"





class cSlime :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSlime(void);

	CLASS_PROTODEF(cSlime);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




