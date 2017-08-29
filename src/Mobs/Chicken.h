#pragma once

#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorCoward.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Behaviors/BehaviorItemDropper.h"
#include "Monster.h"



class cChicken :
	public cMonster
{
	typedef cMonster super;

public:
	cChicken(void);

	CLASS_PROTODEF(cChicken)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_SEEDS);
	}

	virtual void HandleFalling(void) override;

private:
	// Tick controlling behaviors
	cBehaviorBreeder m_BehaviorBreeder;
	cBehaviorItemFollower m_BehaviorItemFollower;
	cBehaviorCoward m_BehaviorCoward;
	cBehaviorWanderer m_BehaviorWanderer;

	cBehaviorItemDropper m_BehaviorItemDropper;
} ;



