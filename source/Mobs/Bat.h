
#pragma once

#include "PassiveMonster.h"





class cBat :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cBat(void);

	virtual bool IsA(const char * a_EntityType) override;
} ;




