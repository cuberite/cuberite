
#pragma once

#include "Mobs/AI/BehaviorTree.h"

namespace BehaviorTree
{

template <eStatus ContinueStatus>
class cBasicComposite:
		public cTask
{
public:

	cBasicComposite(cLuaState & a_LuaState)
	{
		m_Children = CreateChildBehaviours(a_LuaState);
		m_CurrentTask = m_Children.begin();
	}

	cBasicComposite(cTasks a_Children) :
			m_Children(std::move(a_Children)),
			m_CurrentTask(m_Children.begin())
	{
	}

private:
	cTasks m_Children;
	cTasks::iterator m_CurrentTask;

	virtual void DoEntry(cBlackboard & a_Blackboard) override
	{
		m_CurrentTask = m_Children.begin();
	}

	virtual eStatus DoTick(cBlackboard & a_Blackboard) override
	{
		auto Status = ContinueStatus;
		for (; (m_CurrentTask != m_Children.end()); ++m_CurrentTask)
		{
			Status = (*m_CurrentTask)->Tick(a_Blackboard);

			if (Status != ContinueStatus)
			{
				break;
			}
		}
		return Status;
	}

	virtual void DoCancel(cBlackboard & a_Blackboard) override
	{
		if ((*m_CurrentTask)->GetStatus() == eStatus::bsRunning)
		{
			(*m_CurrentTask)->Cancel(a_Blackboard);
		}
	}
};

/** Executes children while they succeed, returns success if all children succeed */
using cSequence = cBasicComposite<eStatus::bsSuccess>;

/** Executes children while they fail, returns success if any child succeeds */
using cSelector = cBasicComposite<eStatus::bsFailure>;

}
