
#pragma once

#include "Monster.h"
#include "../MersenneTwister.h"





class cWitch :
	public cMonster
{
	typedef cMonster super;
	
public:
	cWitch();

	CLASS_PROTODEF(cWitch)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsAngry(void) const {return false; }
} ;




