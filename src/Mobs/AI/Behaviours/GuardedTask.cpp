
#include "Globals.h"
#include "Mobs/AI/Behaviours/GuardedTask.h"

namespace BehaviorTree
{

cGuardedTask::cGuardedTask(cLuaState & a_LuaState)
{
	{
		auto IsGuard = a_LuaState.WalkToValue("Guard");
		if (!IsGuard.IsValid())
		{
			m_Guard = nullptr;
		}
		else
		{
			lua_pushnil(a_LuaState);
			lua_next(a_LuaState, -2);

			auto Name = lua_tostring(a_LuaState, -2);
			m_Guard = CreateBehaviour(Name, a_LuaState);

			a_LuaState.Pop(2);
		}
	}

	{
		auto IsTask = a_LuaState.WalkToValue("Task");
		if (!IsTask.IsValid())
		{
			m_Child = nullptr;
		}
		else
		{
			lua_pushnil(a_LuaState);
			lua_next(a_LuaState, -2);

			auto Name = lua_tostring(a_LuaState, -2);
			m_Child = CreateBehaviour(Name, a_LuaState);
			a_LuaState.Pop(2);
		}
	}
}





bool cGuardedTask::EvaluateGuard(cBlackboard & a_Blackboard)
{
	if (m_Guard == nullptr)
	{
		return true;
	}

	auto Status = m_Guard->Tick(a_Blackboard);
	switch (Status)
	{
		case eStatus::bsSuccess: return true;
		case eStatus::bsFailure: return false;
		default:
		{
			FLOGERROR("Invalid status returned from guard task: {0}", Status);
			m_Guard->Cancel(a_Blackboard);
			return false;
		}
	}
}





eStatus cGuardedTask::DoTick(cBlackboard & a_Blackboard)
{
	if (EvaluateGuard(a_Blackboard))
	{
		return m_Child->Tick(a_Blackboard);
	}

	m_Child->Cancel(a_Blackboard);
	return eStatus::bsFailure;
}

}
