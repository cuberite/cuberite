#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorAttackerMelee.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Behaviors/BehaviorAggressive.h"




class cCaveSpider :
	public cMonster
{
	typedef cMonster super;

public:
	cCaveSpider(void);

	CLASS_PROTODEF(cCaveSpider)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	// tick behaviors
	cBehaviorAttackerMelee m_BehaviorAttackerMelee;
	cBehaviorWanderer m_BehaviorWanderer;

	// other behaviors
	cBehaviorAggressive m_BehaviorAggressive;
} ;




