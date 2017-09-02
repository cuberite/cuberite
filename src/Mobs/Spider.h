
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorAttackerMelee.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Behaviors/BehaviorAggressive.h"
#include "Behaviors/BehaviorDayLightBurner.h"





class cSpider :
	public cMonster
{
	typedef cMonster super;

public:
	cSpider(void);

	CLASS_PROTODEF(cSpider)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
private:
	// tick behaviors
	cBehaviorAttackerMelee m_BehaviorAttackerMelee;
	cBehaviorWanderer m_BehaviorWanderer;

	// other behaviors
	cBehaviorAggressive m_BehaviorAggressive;
} ;




