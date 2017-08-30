
#pragma once

#include "Monster.h"





class cWitch :
	public cMonster
{
	typedef cMonster super;

public:
	cWitch();

	CLASS_PROTODEF(cWitch)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	bool IsAngry() const { return false; }
} ;




