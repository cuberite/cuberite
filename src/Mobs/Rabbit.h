
#pragma once

#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorCoward.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Monster.h"




enum class eRabbitType : UInt8
{
	Brown          = 0,
	White          = 1,
	Black          = 2,
	BlackAndWhite  = 3,
	Gold           = 4,
	SaltAndPepper  = 5,
	TheKillerBunny = 99
};





class cRabbit :
	public cMonster
{
	typedef cMonster super;

public:
	cRabbit();
	cRabbit(eRabbitType Type, int MoreCarrotTicks = 0);

	CLASS_PROTODEF(cRabbit)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_CARROT);
		a_Items.Add(E_ITEM_GOLDEN_CARROT);
		a_Items.Add(E_BLOCK_DANDELION);
	}

	eRabbitType GetRabbitType()         const { return m_Type; }
	int         GetMoreCarrotTicks()    const { return m_MoreCarrotTicks; }

	virtual cBehaviorBreeder * GetBehaviorBreeder() override;
	virtual const cBehaviorBreeder * GetBehaviorBreeder() const override;
private:
	// Tick controlling behaviors
	cBehaviorBreeder m_BehaviorBreeder;
	cBehaviorItemFollower m_BehaviorItemFollower;
	cBehaviorCoward m_BehaviorCoward;
	cBehaviorWanderer m_BehaviorWanderer;

	eRabbitType m_Type;
	int m_MoreCarrotTicks;  // Ticks until the Rabbit eat planted Carrots
} ;
