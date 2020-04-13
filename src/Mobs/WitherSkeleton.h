
#pragma once

#include "AggressiveMonster.h"





class cWitherSkeleton:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cWitherSkeleton();

	CLASS_PROTODEF(cWitherSkeleton)

	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

} ;




