
#include "RandomPosition.h"

#include "FastRandom.h"
#include "Mobs/Monster.h"

namespace BehaviorTree
{
cRandomPosition::cRandomPosition(cLuaState & a_LuaState)
{
	if (!a_LuaState.GetNamedValue("UniqueID", m_Key))
	{
		LOGWARNING("cRandomPosition is missing parameter UniqueID. Using Defaul: \"RandomPosition\".");
		m_Key = "RandomPosition";
	}
	if (!a_LuaState.GetNamedValue("Range", m_MaxRange))
	{
		LOGWARNING("cRandomPosition is missing parameter Range. Setting to 10!");
		m_MaxRange = 10;
	}
}





eStatus cRandomPosition::DoTick(cBlackboard & a_Blackboard)
{
	auto & Random = GetRandomProvider();
	Vector3d Dist;
	Dist.x = Random.RandReal(-m_MaxRange, m_MaxRange);
	Dist.z = Random.RandReal(-m_MaxRange, m_MaxRange);

	Vector3d Destination = a_Blackboard.Self().GetPosition() + Dist;
	Destination.y = Clamp(Destination.y, 0.0, static_cast<double>(cChunkDef::Height));

	Json::Value Position;
	Position["x"] = Destination.x;
	Position["y"] = Destination.y;
	Position["z"] = Destination.z;

	a_Blackboard[m_Key] = std::move(Position);

	return eStatus::bsSuccess;
}
}
