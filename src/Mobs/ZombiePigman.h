#pragma once

#include "PassiveAggressiveMonster.h"





class cZombiePigman:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cZombiePigman();

	CLASS_PROTODEF(cZombiePigman)

private:

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool IsUndead(void) override { return true; }
	virtual bool IsNetherNative(void) override { return true; }
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
} ;
