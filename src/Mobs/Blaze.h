#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorAttackerRanged.h"
#include "Behaviors/BehaviorDoNothing.h"
#include "Behaviors/BehaviorAggressive.h"

class cBlaze :
	public cMonster
{
	typedef cMonster super;

public:
	cBlaze(void);

	CLASS_PROTODEF(cBlaze)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

private:
	// tick behaviors
	cBehaviorAttackerRanged m_BehaviorAttackerRanged;
	cBehaviorDoNothing m_BehaviorDoNothing;

	// other behaviors
	cBehaviorAggressive m_BehaviorAggressive;
} ;
