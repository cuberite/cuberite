
#pragma once

#include "AggressiveMonster.h"





class cGhast :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cGhast(void);

	CLASS_PROTODEF(cGhast);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Attack(float a_Dt) override;

	bool IsCharging(void) const {return false; }
} ;




