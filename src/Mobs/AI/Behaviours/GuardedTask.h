
#pragma once

#include "Globals.h"
#include "Mobs/AI/BehaviorTree.h"

namespace BehaviorTree
{

/** A task that should only tick when its guard task allows it */
class cGuardedTask :
		public cTask
{
public:
	cGuardedTask(cLuaState & a_LuaState);

	cGuardedTask(std::unique_ptr<cTask> a_Guard, std::unique_ptr<cTask> a_Child) :
			m_Guard(std::move(a_Guard)),
			m_Child(std::move(a_Child))
	{
		ASSERT(m_Child != nullptr);
	}

private:
	std::unique_ptr<cTask> m_Guard, m_Child;

	bool EvaluateGuard(cBlackboard & a_Blackboard);
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;

	friend class cDynamicGuardSelector;
};

}
