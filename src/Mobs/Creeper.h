
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorAttackerSuicideBomber.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Behaviors/BehaviorAggressive.h"
#include "Behaviors/BehaviorDayLightBurner.h"



class cCreeper :
	public cMonster
{
	typedef cMonster super;

public:
	cCreeper(void);

	CLASS_PROTODEF(cCreeper)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	bool IsBlowing(void) const;
	bool IsCharged(void) const;
	bool IsBurnedWithFlintAndSteel(void) const;

private:
	// tick behaviors
	cBehaviorAttackerSuicideBomber m_BehaviorAttackerSuicideBomber;
	cBehaviorWanderer m_BehaviorWanderer;

	// other behaviors
	cBehaviorAggressive m_BehaviorAggressive;
} ;




