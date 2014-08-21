
#pragma once

#include "NewMonster.h"





class cNewGhast :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewGhast(void);

	CLASS_PROTODEF(cNewGhast)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsCharging(void) const {return false; }
} ;




