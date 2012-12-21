
#pragma once

#include "PassiveMonster.h"





class cOcelot :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cOcelot();

	virtual bool IsA(const char * a_EntityType) override;
} ;




