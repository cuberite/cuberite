
#pragma once

#include "AggressiveMonster.h"





class cEnderDragon :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cEnderDragon(void);

	CLASS_PROTODEF(cEnderDragon)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;




