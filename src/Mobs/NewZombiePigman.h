#pragma once

#include "NewMonster.h"





class cNewZombiePigman :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewZombiePigman(void);

	CLASS_PROTODEF(cNewZombiePigman)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




