
#pragma once

#include "AggressiveMonster.h"





class cEnderDragon:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cEnderDragon();

	CLASS_PROTODEF(cEnderDragon)

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void SpawnOn(cClientHandle & a_Client) override;
} ;
