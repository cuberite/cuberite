
#pragma once

#include "AggressiveMonster.h"





class cSkeleton :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cSkeleton(bool IsWither);

	CLASS_PROTODEF(cSkeleton)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual bool IsUndead(void) override { return true; }

	bool IsWither(void) const { return m_bIsWither; }

private:

	bool m_bIsWither;

} ;




