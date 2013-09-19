
#pragma once

#include "AggressiveMonster.h"





class cIronGolem :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cIronGolem(void);

	CLASS_PROTODEF(cIronGolem);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




