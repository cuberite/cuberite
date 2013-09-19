
#pragma once

#include "AggressiveMonster.h"





class cWither :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cWither(void);

	CLASS_PROTODEF(cWither);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




