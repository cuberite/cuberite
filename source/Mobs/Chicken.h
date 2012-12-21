
#pragma once

#include "PassiveMonster.h"





class cChicken :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cChicken(void);

	CLASS_PROTODEF(cChicken);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




