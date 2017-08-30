#pragma once

#include "Monster.h"

class cBlaze :
	public cMonster
{
	typedef cMonster super;

public:
	cBlaze(void);

	CLASS_PROTODEF(cBlaze)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;
