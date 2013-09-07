
#pragma once

#include "PassiveMonster.h"





class cBat :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cBat(void);

	CLASS_PROTODEF(cBat);
	
	virtual eFamily GetMobFamily(void) const override;
} ;




