
#pragma once

#include "NewMonster.h"





class cNewSnowGolem :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewSnowGolem(void);

	CLASS_PROTODEF(cNewSnowGolem)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




