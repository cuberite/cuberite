
#pragma once

#include "PassiveMonster.h"





class cBat :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cBat(void) :
		// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
		super("Bat", 65, "mob.bat.hurt", "mob.bat.death", 0.7, 0.7)
	{
	}

	CLASS_PROTODEF(cBat);

	bool IsHanging(void) const {return false; }
} ;




