
#pragma once

#include "AggressiveMonster.h"





class cMagmacube :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cMagmacube();

	CLASS_PROTODEF(cMagmacube);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




