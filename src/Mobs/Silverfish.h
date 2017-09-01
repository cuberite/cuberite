
#pragma once

#include "Monster.h"





class cSilverfish :
	public cMonster
{
	typedef cMonster super;

public:
	cSilverfish(void) :
		super(mtSilverfish, "entity.silverfish.hurt", "entity.silverfish.death", 0.3, 0.7)
	{
		GetMonsterConfig("Silverfish");
	}

	CLASS_PROTODEF(cSilverfish)
} ;




