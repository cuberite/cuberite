
#pragma once

#include "Monster.h"





class cOcelot :
	public cMonster
{
	typedef cMonster super;
	
public:
	cOcelot(void) :
		super("Ocelot", mtOcelot, "mob.cat.hitt", "mob.cat.hitt", 0.6, 0.8)
	{
	}

	CLASS_PROTODEF(cOcelot)
} ;




