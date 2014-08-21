
#pragma once

#include "NewMonster.h"





class cNewIronGolem :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewIronGolem(void);

	CLASS_PROTODEF(cNewIronGolem)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




