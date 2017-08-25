#pragma once

#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorItemReplacer.h"
#include "Behaviors/BehaviorCoward.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Monster.h"

class cCow : public cMonster
{
public:
	cCow();
	~cCow() {}

	typedef cMonster super;
	CLASS_PROTODEF(cCow)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_WHEAT);
	}

	virtual cBehaviorBreeder * GetBehaviorBreeder() override;
	virtual const cBehaviorBreeder * GetBehaviorBreeder() const override;
private:
	// Tick controlling behaviors
	cBehaviorBreeder m_BehaviorBreeder;
	cBehaviorItemFollower m_BehaviorItemFollower;
	cBehaviorCoward m_BehaviorCoward;
	cBehaviorItemReplacer m_BehaviorWanderer;

	// Non tick controlling behaviors
	cBehaviorWanderer m_BehaviorItemReplacer;
} ;




