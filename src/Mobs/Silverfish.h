
#pragma once

#include "Monster.h"





class cSilverfish :
	public cMonster
{
	typedef cMonster super;
	
public:
	cSilverfish(void) :
		super("Silverfish", mtSilverfish, "mob.silverfish.hit", "mob.silverfish.kill", 0.3, 0.7)
	{
	}

	CLASS_PROTODEF(cSilverfish)
} ;




