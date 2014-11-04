
#pragma once

#include "AggressiveMonster.h"





class cGiant :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cGiant(CreateMonsterInfo a_Info);

	CLASS_PROTODEF(cGiant)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




