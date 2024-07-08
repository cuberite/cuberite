
#pragma once

#include "Globals.h"

#include "Mobs/AI/Behaviours/GuardedTask.h"

namespace BehaviorTree
{

/** A selector that always runs the active task with the highest priority, cancelling running tasks as necessary */
class cDynamicGuardSelector:
		public cTask
{
	using cGuardedTasks = std::vector<std::unique_ptr<cGuardedTask>>;
public:
	cDynamicGuardSelector(cGuardedTasks a_Children) :
			m_Children(std::move(a_Children)),
			m_CurrentTask(m_Children.begin())
	{
	}

	cDynamicGuardSelector(cLuaState & a_LuaState);

private:
	cGuardedTasks m_Children;
	cGuardedTasks::iterator m_CurrentTask;

	virtual void DoEntry(cBlackboard & a_Blackboard) override;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};

}
