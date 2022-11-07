
#include "RandomWait.h"

#include "World.h"
#include "Mobs/Monster.h"

namespace BehaviorTree
{
cRandomWait::cRandomWait(cLuaState & a_LuaState)
{
	int Min = 0, Max = 1;
	if (!a_LuaState.GetNamedValue("Min", Min))
	{
		LOGWARNING("cRandomWait is missing parameter Min. Using Default: 0.");
	}
	m_WaitMin = std::chrono::seconds(Min);

	if (!a_LuaState.GetNamedValue("Max", Max))
	{
		LOGWARNING("cRandomWait is missing parameter Max. Using Default: 1.");
	}
	m_WaitMax = std::chrono::seconds(Max);
}





void cRandomWait::DoEntry(cBlackboard & a_Blackboard)
{
	cTickTimeLong CurrentAge(a_Blackboard.Self().GetWorld()->GetWorldAge());
	cTickTime Wait(GetRandomProvider().RandInt(m_WaitMin.count(), m_WaitMax.count()));

	BT_LOG("Waiting for {0} ticks", Wait.count());
	m_WaitEnd = CurrentAge + Wait;
}





eStatus cRandomWait::DoTick(cBlackboard & a_Blackboard)
{
	cTickTimeLong CurrentAge(a_Blackboard.Self().GetWorld()->GetWorldAge());

	return (m_WaitEnd <= CurrentAge) ? eStatus::bsSuccess : eStatus::bsRunning;
}
}
