
#pragma once

#include "NewMonster.h"





class cNewOcelot :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewOcelot(void) :
		super("Ocelot", mtOcelot, "mob.cat.hitt", "mob.cat.hitt", 0.6, 0.8)
	{
	}

	CLASS_PROTODEF(cNewOcelot)
} ;




