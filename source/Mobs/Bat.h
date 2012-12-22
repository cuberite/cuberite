
#pragma once

#include "PassiveMonster.h"





class cBat :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cBat(void) :
		super("Bat", 65, "mob.bat.hurt", "mob.bat.death")
	{
	}

	CLASS_PROTODEF(cBat);
} ;




