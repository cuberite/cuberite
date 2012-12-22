
#pragma once

#include "PassiveMonster.h"





class cOcelot :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cOcelot(void) :
		super("Ocelot", 98, "mob.cat.hitt", "mob.cat.hitt")
	{
	}

	CLASS_PROTODEF(cOcelot);
} ;




