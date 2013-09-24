
#pragma once

#include "AggressiveMonster.h"





class cHorse :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cHorse(void);

	CLASS_PROTODEF(cHorse);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




