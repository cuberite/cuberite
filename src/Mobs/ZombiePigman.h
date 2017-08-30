#pragma once

#include "Monster.h"





class cZombiePigman :
	public cMonster
{
	typedef cMonster super;

public:
	cZombiePigman(void);

	CLASS_PROTODEF(cZombiePigman)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual bool IsUndead(void) override { return true; }
} ;




