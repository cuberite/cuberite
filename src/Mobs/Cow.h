
#pragma once

#include "PassiveMonster.h"
#include "AI/BehaviorTree.h"





class cCow :
	public cPassiveMonster
{
	typedef cPassiveMonster super;

public:
	cCow();

	CLASS_PROTODEF(cCow)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_WHEAT);
	}

	virtual void InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

private:
	std::unique_ptr<BehaviorTree::cTask> m_BehaviorTree;
	cBlackboard m_Blackboard;
} ;




