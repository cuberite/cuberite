#pragma once

#include "PassiveAggressiveMonster.h"





class cZombiePigman :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cZombiePigman(void);

	CLASS_PROTODEF(cZombiePigman);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void KilledBy(cEntity * a_Killer) override;
} ;




