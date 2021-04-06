
#pragma once

#include "AggressiveMonster.h"





class cSilverfish:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cSilverfish();

	CLASS_PROTODEF(cSilverfish)

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
} ;
