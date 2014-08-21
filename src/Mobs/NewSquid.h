
#pragma once

#include "NewMonster.h"





class cNewSquid :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewSquid();

	CLASS_PROTODEF(cNewSquid)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




