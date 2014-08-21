
#pragma once

#include "NewMonster.h"





class cNewBat :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewBat(void);

	CLASS_PROTODEF(cNewBat)
	
	bool IsHanging(void) const {return false; }
} ;




