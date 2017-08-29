
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorDoNothing.h"





class cSquid :
	public cMonster
{
	typedef cMonster super;

public:
	cSquid();

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	CLASS_PROTODEF(cSquid)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	// Squids do not drown (or float)
	virtual void HandleAir(void) override {}
	virtual void SetSwimState(cChunk & a_Chunk) override {}

private:
	cBehaviorDoNothing m_BehaviorDoNothing;
} ;




