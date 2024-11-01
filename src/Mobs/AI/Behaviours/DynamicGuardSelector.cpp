
#include "DynamicGuardSelector.h"

namespace BehaviorTree
{
cDynamicGuardSelector::cDynamicGuardSelector(cLuaState & a_LuaState)
{
	lua_pushnil(a_LuaState);
	while (lua_next(a_LuaState, -2) != 0)
	{
		if (lua_istable(a_LuaState, -1))
		{
			m_Children.push_back(std::make_unique<cGuardedTask>(a_LuaState));
		}
		a_LuaState.Pop(1);
	}
	m_CurrentTask = m_Children.begin();
}





void cDynamicGuardSelector::DoEntry(cBlackboard & a_Blackboard)
{
	m_CurrentTask = m_Children.begin();
}





eStatus cDynamicGuardSelector::DoTick(cBlackboard & a_Blackboard)
{
	eStatus Status = eStatus::bsFailure;
	auto First  = m_Children.begin();
	const auto Last   = m_Children.end();

	for (;;)
	{
		First = std::find_if(First, Last, [&](const std::unique_ptr<cGuardedTask> & a_Behavior)
		{
			return a_Behavior->EvaluateGuard(a_Blackboard);
		}
		);

		if ((First != m_CurrentTask) && (m_CurrentTask != Last))
		{
			m_CurrentTask->get()->Cancel(a_Blackboard);
			m_CurrentTask = Last;
		}

		if (First == Last)
		{
			break;
		}

		Status = First->get()->m_Child->Tick(a_Blackboard);
		if (Status != eStatus::bsFailure)
		{
			m_CurrentTask = First;
			break;
		}
	}
	return Status;
}
}
