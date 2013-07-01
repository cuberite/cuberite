
#pragma once

#include "PassiveAggressiveMonster.h"





class cWolf :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cWolf(void) :
		// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here (wiki.vg values are suspicious)
		super("Wolf", 95, "mob.wolf.hurt", "mob.wolf.death", 0.9, 0.9)
	{
	}

	CLASS_PROTODEF(cWolf);
} ;




