
#pragma once

#include "PassiveMonster.h"
#include "Ageable.h"




class cOcelot :
	public cPassiveMonster, public Ageable
{
	typedef cPassiveMonster super;
	
public:
	cOcelot() :
		super("Ocelot", mtOcelot, "mob.cat.hitt", "mob.cat.hitt", 0.6, 0.8)
	{
	}

	CLASS_PROTODEF(cOcelot)
} ;




