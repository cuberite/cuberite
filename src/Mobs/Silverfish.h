
#pragma once

#include "AggressiveMonster.h"





class cSilverfish :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cSilverfish(void) :
		super("Silverfish", mtSilverfish, "mob.silverfish.hit", "mob.silverfish.kill", 0.3, 0.7)
	{
	}

	CLASS_PROTODEF(cSilverfish)
} ;




