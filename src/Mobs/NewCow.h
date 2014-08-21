
#pragma once

#include "NewMonster.h"





class cNewCow :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewCow();

	CLASS_PROTODEF(cNewCow)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




