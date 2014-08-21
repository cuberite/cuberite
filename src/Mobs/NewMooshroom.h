
#pragma once

#include "NewMonster.h"





class cNewMooshroom :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewMooshroom(void);

	CLASS_PROTODEF(cNewMooshroom)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




