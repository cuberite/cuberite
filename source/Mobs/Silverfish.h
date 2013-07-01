
#pragma once

#include "AggressiveMonster.h"





class cSilverfish :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSilverfish(void) :
		// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
		super("Silverfish", 60, "mob.silverfish.hit", "mob.silverfish.kill", 0.9, 0.3)
	{
	}

	CLASS_PROTODEF(cSilverfish);
} ;




