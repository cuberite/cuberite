
#pragma once

#include "AggressiveMonster.h"





class cCreeper :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cCreeper(void);

	CLASS_PROTODEF(cCreeper);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




