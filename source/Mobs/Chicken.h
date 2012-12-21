
#pragma once

#include "PassiveMonster.h"





class cChicken :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cChicken(void);

	virtual bool IsA(const char * a_EntityType) override;

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




