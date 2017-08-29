
#pragma once

#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorCoward.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Monster.h"





class cMooshroom :
	public cMonster
{
	typedef cMonster super;

public:
	cMooshroom(void);

	CLASS_PROTODEF(cMooshroom)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

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
} ;




