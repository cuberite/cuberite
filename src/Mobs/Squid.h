
#pragma once

#include "Monster.h"





class cSquid :
	public cMonster
{
	typedef cMonster super;
	
public:
	cSquid();

	CLASS_PROTODEF(cSquid)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




