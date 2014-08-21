#pragma once

#include "Monster.h"





class cZombiePigman :
	public cMonster
{
	typedef cMonster super;
	
public:
	cZombiePigman(void);

	CLASS_PROTODEF(cZombiePigman)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) /*override*/;
} ;




