
#pragma once

#include "NewMonster.h"





class cNewSilverfish :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewSilverfish(void) :
		super("Silverfish", mtSilverfish, "mob.silverfish.hit", "mob.silverfish.kill", 0.3, 0.7)
	{
	}

	CLASS_PROTODEF(cNewSilverfish)
} ;




