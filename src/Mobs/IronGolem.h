
#pragma once

#include "PassiveAggressiveMonster.h"





class cIronGolem:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cIronGolem();

	CLASS_PROTODEF(cIronGolem)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	// Iron golems do not drown nor float
	virtual void HandleAir(void) override {}
	virtual void SetSwimState(cChunk & a_Chunk) override {}
} ;




