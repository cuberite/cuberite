
#pragma once

#include "PassiveMonster.h"





class cOcelot :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cOcelot(void) :
		// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
		super("Ocelot", 98, "mob.cat.hitt", "mob.cat.hitt", 0.9, 0.5)
	{
	}

	CLASS_PROTODEF(cOcelot);
} ;




