
#pragma once

#include "AggressiveMonster.h"





class cBlaze :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cBlaze(void);

	CLASS_PROTODEF(cBlaze)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
} ;
