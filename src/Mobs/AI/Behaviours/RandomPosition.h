
#pragma once

#include "Globals.h"
#include "Mobs/AI/BehaviorTree.h"

namespace BehaviorTree
{
/** Writes a random position around the monster onto the blackboard */
class cRandomPosition :
		public cTask
{
public:
	cRandomPosition(cBlackboardKey a_WriteKey, double a_MaxRange) :
			m_Key(std::move(a_WriteKey)),
			m_MaxRange(a_MaxRange)
	{
	}

	cRandomPosition(cLuaState & a_LuaState);

private:
	cBlackboardKey m_Key;
	double m_MaxRange;

	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};
}
