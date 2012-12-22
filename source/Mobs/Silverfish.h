
#pragma once

#include "AggressiveMonster.h"





class cSilverfish :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSilverfish(void) :
		super("Silverfish", 60, "mob.silverfish.hit", "mob.silverfish.kill")
	{
	}

	CLASS_PROTODEF(cSilverfish);
} ;




