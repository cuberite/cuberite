
#pragma once

#include "AggressiveMonster.h"





class cGiant :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cGiant(void);

	CLASS_PROTODEF(cGiant)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;




