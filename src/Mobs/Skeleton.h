
#pragma once

#include "AggressiveMonster.h"





class cSkeleton:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cSkeleton();

	CLASS_PROTODEF(cSkeleton)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual bool IsUndead(void) override { return true; }

} ;




