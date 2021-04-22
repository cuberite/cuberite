#pragma once

#include "AggressiveMonster.h"





class cZombie:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cZombie();

	CLASS_PROTODEF(cZombie)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool IsUndead(void) override { return true; }

} ;




