
#include "HealthRange.h"

#include "Mobs/Monster.h"

namespace BehaviorTree
{
	cHealthRange::cHealthRange(cLuaState & a_LuaState)
	{
		if (!a_LuaState.GetNamedValue("Max", m_MaxHealth))
		{
			LOGWARNING("cHealthRange is missing parameter Max. Setting to 1.0f!");
			m_MaxHealth = 1.0f;
		}
		if (!a_LuaState.GetNamedValue("Min", m_MinHealth))
		{
			LOGWARNING("cHealthRange is missing parameter Max. Setting to 0.0f!");
			m_MinHealth = 0;
		}
	}





	eStatus cHealthRange::DoTick(cBlackboard & a_Blackboard)
	{
		auto Health = a_Blackboard.Self().GetHealth();
		if ((Health > m_MinHealth) && (Health < m_MaxHealth * a_Blackboard.Self().GetMaxHealth()))
		{
			return eStatus::bsSuccess;
		}

		return eStatus::bsFailure;
	}
}
