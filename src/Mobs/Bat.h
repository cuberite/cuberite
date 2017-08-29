
#pragma once

#include "Monster.h"





class cBat :
	public cMonster
{
	typedef cMonster super;

public:
	cBat(void);

	CLASS_PROTODEF(cBat)

	bool IsHanging(void) const {return false; }
} ;




