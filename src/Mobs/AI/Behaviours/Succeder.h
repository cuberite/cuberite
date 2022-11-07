
#pragma once

#include "Globals.h"
#include "Mobs/AI/BehaviorTree.h"

namespace BehaviorTree
{

/** A task decorator that turns failure into success */
class cSucceeder:
		public cTask
{
public:
	cSucceeder(std::unique_ptr<cTask> a_Child):
			m_Child(std::move(a_Child))
	{
		ASSERT(m_Child != nullptr);
	}

	cSucceeder(cLuaState & a_LuaState);

private:
	std::unique_ptr<cTask> m_Child;

	virtual eStatus DoTick(cBlackboard & a_Blackboard) override
	{
		auto ChildStatus = m_Child->Tick(a_Blackboard);
		return (ChildStatus == eStatus::bsRunning) ? eStatus::bsRunning : eStatus::bsSuccess;
	}

};
}
