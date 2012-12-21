
#pragma once

#include "PassiveMonster.h"





class cMooshroom :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cMooshroom(void);

	virtual bool IsA(const char * a_EntityType);

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




