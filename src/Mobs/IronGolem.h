
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorWanderer.h"




class cIronGolem :
	public cMonster
{
	typedef cMonster super;

public:
	cIronGolem(void);

	CLASS_PROTODEF(cIronGolem)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	// Iron golems do not drown nor float
	virtual void HandleAir(void) override {}
	virtual void SetSwimState(cChunk & a_Chunk) override {}

private:
	cBehaviorWanderer m_BehaviorWanderer;
} ;




