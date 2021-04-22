
#pragma once

#include "PassiveMonster.h"





class cBat:
	public cPassiveMonster
{
	using Super = cPassiveMonster;

public:

	cBat();

	CLASS_PROTODEF(cBat)

	bool IsHanging(void) const {return false; }
} ;




