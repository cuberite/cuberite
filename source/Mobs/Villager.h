
#pragma once

#include "PassiveMonster.h"





class cVillager :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cVillager();

	virtual bool IsA(const char * a_EntityType) override;
} ;




