
#pragma once

#include "AggressiveMonster.h"





class cWitch :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cWitch();

	CLASS_PROTODEF(cWitch)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	bool IsAngry() const { return false; }
} ;




