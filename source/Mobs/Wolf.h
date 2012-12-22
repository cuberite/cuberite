
#pragma once

#include "PassiveAggressiveMonster.h"





class cWolf :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cWolf(void) :
		super("Wolf", 95, "mob.wolf.hurt", "mob.wolf.death")
	{
	}

	CLASS_PROTODEF(cWolf);
} ;




