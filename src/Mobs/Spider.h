
#pragma once

#include "AggressiveMonster.h"





class cSpider :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSpider(CreateMonsterInfo a_Info);

	CLASS_PROTODEF(cSpider)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




