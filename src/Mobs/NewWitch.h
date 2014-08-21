
#pragma once

#include "NewMonster.h"
#include "../MersenneTwister.h"





class cNewWitch :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewWitch();

	CLASS_PROTODEF(cNewWitch)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsAngry(void) const {return false; }
} ;




