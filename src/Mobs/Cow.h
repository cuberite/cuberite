#pragma once

#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorCoward.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Behaviors/BehaviorItemReplacer.h"
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
	cBehaviorWanderer m_BehaviorWanderer;

	// Non tick controlling behaviors
	cBehaviorItemReplacer m_BehaviorItemReplacer;
} ;




