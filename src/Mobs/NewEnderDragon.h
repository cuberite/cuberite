
#pragma once

#include "NewMonster.h"





class cNewEnderDragon :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewEnderDragon(void);

	CLASS_PROTODEF(cNewEnderDragon)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




