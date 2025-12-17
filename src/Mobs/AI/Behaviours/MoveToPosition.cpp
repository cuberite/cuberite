
#include "MoveToPosition.h"

#include "Mobs/Monster.h"

namespace BehaviorTree
{

cMoveToPosition::cMoveToPosition(cLuaState & a_LuaState)
{
	if (!a_LuaState.GetNamedValue("UniqueID", m_Key))
	{
		LOGWARNING("cMoveToPosition is missing parameter UniqueID. Using Default: \"RandomPosition\".");
		m_Key = "RandomPosition";
	}
}





void cMoveToPosition::DoEntry(cBlackboard & a_Blackboard)
{
	Vector3d Position;
	auto & JsonPos = a_Blackboard[m_Key];
	Position.x = JsonPos["x"].asDouble();
	Position.y = JsonPos["y"].asDouble();
	Position.z = JsonPos["z"].asDouble();

	BT_LOG("Moving to position ({0}, {1}, {2})", Position.x, Position.y, Position.z);
	a_Blackboard.Self().MoveToPosition(Position);
}





eStatus cMoveToPosition::DoTick(cBlackboard & a_Blackboard)
{
	auto & Self = a_Blackboard.Self();
	if (Self.ReachedFinalDestination())
	{
		BT_LOG("Reached destination");
		return eStatus::bsSuccess;
	}

	if (!Self.m_PathfinderActivated)
	{
		// Path not found
		BT_LOG("Could not reach destination");
		return eStatus::bsFailure;
	}

	// We're on our way
	return eStatus::bsRunning;
}





void cMoveToPosition::DoExit(cBlackboard & a_Blackboard)
{
	a_Blackboard.Self().StopMovingToPosition();
}





void cMoveToPosition::DoCancel(cBlackboard & a_Blackboard)
{
	a_Blackboard.Self().StopMovingToPosition();
}

}
