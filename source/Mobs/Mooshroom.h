
#pragma once

#include "PassiveMonster.h"





class cMooshroom :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cMooshroom(void);

	CLASS_PROTODEF(cMooshroom);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




