
#pragma once

#include "AggressiveMonster.h"





class cPassiveAggressiveMonster :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cPassiveAggressiveMonster(void);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
} ;




