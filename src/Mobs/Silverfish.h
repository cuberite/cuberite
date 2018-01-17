
#pragma once

#include "AggressiveMonster.h"





class cSilverfish :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cSilverfish(void) :
		super("Silverfish", mtSilverfish, "entity.silverfish.hurt", "entity.silverfish.death", 0.3, 0.4)
	{
	}

	CLASS_PROTODEF(cSilverfish)
} ;




