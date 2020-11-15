
#pragma once

#include "AggressiveMonster.h"





class cSilverfish:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cSilverfish():
		Super("Silverfish", mtSilverfish, "entity.silverfish.hurt", "entity.silverfish.death", "entity.silverfish.ambient", 0.3, 0.4)
	{
	}
	CLASS_PROTODEF(cSilverfish)

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
} ;




