
#pragma once

#include "Monster.h"





class cSilverfish :
	public cMonster
{
	typedef cMonster super;

public:
	cSilverfish(void) :
		super("Silverfish", mtSilverfish, "entity.silverfish.hurt", "entity.silverfish.death", 0.3, 0.7)
	{
	}

	CLASS_PROTODEF(cSilverfish)
} ;




