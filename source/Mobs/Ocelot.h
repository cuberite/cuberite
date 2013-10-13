
#pragma once

#include "PassiveMonster.h"





class cOcelot :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cOcelot(void) :
		super("Ocelot", 98, "mob.cat.hitt", "mob.cat.hitt", 0.6, 0.8)
	{
	}

	CLASS_PROTODEF(cOcelot);
} ;




