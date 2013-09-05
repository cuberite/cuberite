
#pragma once

#include "AggressiveMonster.h"





class cSkeleton :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSkeleton();

	CLASS_PROTODEF(cSkeleton);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




