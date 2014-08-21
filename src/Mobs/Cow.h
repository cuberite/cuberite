
#pragma once

#include "Monster.h"





class cCow :
	public cMonster
{
	typedef cMonster super;
	
public:
	cCow();

	CLASS_PROTODEF(cCow)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) /*override*/;
} ;




