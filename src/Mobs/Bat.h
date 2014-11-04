
#pragma once

#include "PassiveMonster.h"





class cBat :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cBat(CreateMonsterInfo a_Info);

	CLASS_PROTODEF(cBat)
	
	bool IsHanging(void) const {return false; }
} ;




