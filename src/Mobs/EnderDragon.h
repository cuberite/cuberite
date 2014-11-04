
#pragma once

#include "AggressiveMonster.h"





class cEnderDragon :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cEnderDragon(CreateMonsterInfo a_Info);

	CLASS_PROTODEF(cEnderDragon)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




