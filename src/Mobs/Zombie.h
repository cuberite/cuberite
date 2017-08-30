#pragma once

#include "Monster.h"

#include "Behaviors/BehaviorAttackerMelee.h"
#include "Behaviors/BehaviorWanderer.h"

#include "Behaviors/BehaviorAggressive.h"
#include "Behaviors/BehaviorDayLightBurner.h"




class cZombie :
	public cMonster
{
	typedef cMonster super;

public:
	cZombie(bool a_IsVillagerZombie);

	CLASS_PROTODEF(cZombie)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool IsUndead(void) override { return true; }

	bool IsVillagerZombie(void) const { return m_IsVillagerZombie; }
	bool IsConverting    (void) const { return m_IsConverting; }
private:

	bool m_IsVillagerZombie;
	bool m_IsConverting;

	// tick behaviors
	cBehaviorAttackerMelee m_BehaviorAttackerMelee;
	cBehaviorWanderer m_BehaviorWanderer;

	// other behaviors
	cBehaviorAggressive m_BehaviorAggressive;
	cBehaviorDayLightBurner m_BehaviourDayLightBurner;
} ;




