
#pragma once

#include "NewMonster.h"





class cNewBlaze :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewBlaze(void);

	CLASS_PROTODEF(cNewBlaze)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;
