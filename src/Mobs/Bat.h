
#pragma once

#include "PassiveMonster.h"





class cBat : public cMonster
{

public:
	cBat(void);

	CLASS_PROTODEF(cBat)

	bool IsHanging(void) const {return false; }
} ;




