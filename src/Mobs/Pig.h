#pragma once


#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorCoward.h"
#include "Behaviors/BehaviorWanderer.h"
#include "Monster.h"

class cPig :
	public cMonster
{
	typedef cMonster super;

public:
	cPig(void);

	CLASS_PROTODEF(cPig)

	// cEntity overrides
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_CARROT);
	}

	bool IsSaddled(void) const { return m_bIsSaddled; }
private:
	// Tick controlling behaviors
	cBehaviorBreeder m_BehaviorBreeder;
	cBehaviorItemFollower m_BehaviorItemFollower;
	cBehaviorCoward m_BehaviorCoward;
	cBehaviorWanderer m_BehaviorWanderer;

	bool m_bIsSaddled;

} ;




