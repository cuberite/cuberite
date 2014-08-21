
#pragma once

#include "Monster.h"





class cIronGolem :
	public cMonster
{
	typedef cMonster super;
	
public:
	cIronGolem(void);

	CLASS_PROTODEF(cIronGolem)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) /*override*/;
} ;




