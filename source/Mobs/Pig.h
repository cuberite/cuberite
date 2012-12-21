
#pragma once

#include "PassiveMonster.h"





class cPig :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cPig(void);

	CLASS_PROTODEF(cPig);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




