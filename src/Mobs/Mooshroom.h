
#pragma once

#include "Monster.h"





class cMooshroom :
	public cMonster
{
	typedef cMonster super;
	
public:
	cMooshroom(void);

	CLASS_PROTODEF(cMooshroom)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) /*override*/;
} ;




