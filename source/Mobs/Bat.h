
#pragma once

#include "PassiveMonster.h"





class cBat :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cBat(void) :
		super("Bat", 65, "mob.bat.hurt", "mob.bat.death", 0.5, 0.9)
	{
	}

	CLASS_PROTODEF(cBat);

	bool IsHanging(void) const {return false; }
} ;




