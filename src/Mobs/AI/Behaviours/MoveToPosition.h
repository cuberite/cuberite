
#pragma once

#include "Globals.h"
#include "Mobs/AI/BehaviorTree.h"

namespace BehaviorTree
{

/** Move to a position in the world */
class cMoveToPosition:
		public cTask
{
public:
	cMoveToPosition(cBlackboardKey a_PositionKey) :
			m_Key(std::move(a_PositionKey))
	{
	}

	cMoveToPosition(cLuaState & a_LuaState);

private:
	cBlackboardKey m_Key;

	virtual void DoEntry(cBlackboard & a_Blackboard) override;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
	virtual void DoExit(cBlackboard & a_Blackboard) override;
	virtual void DoCancel(cBlackboard & a_Blackboard) override;
};

}
