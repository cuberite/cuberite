
#pragma once

#include "Globals.h"
#include "Mobs/AI/BehaviorTree.h"

namespace BehaviorTree
{
/** Wait a random amount of time */
class cRandomWait:
		public cTask
{
public:
	cRandomWait(cLuaState & a_LuaState);

	cRandomWait(cTickTime a_WaitMin, cTickTime a_WaitMax):
			m_WaitMin(a_WaitMin),
			m_WaitMax(a_WaitMax)
	{
	}

private:
	cTickTime m_WaitMin, m_WaitMax;
	cTickTimeLong m_WaitEnd;

	virtual void DoEntry(cBlackboard & a_Blackboard) override;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};
}
