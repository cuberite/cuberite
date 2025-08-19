
#pragma once

#include "Globals.h"
#include "Mobs/AI/BehaviorTree.h"

namespace BehaviorTree
{
/** Succeeds if the monster's health is within some range */
class cHealthRange:
	public cTask
{
public:
	cHealthRange(float a_MinHealth, float a_MaxHealth):
		m_MinHealth(a_MinHealth),
		m_MaxHealth(a_MaxHealth)
	{
	}

	cHealthRange(cLuaState & a_LuaState);

private:
	float m_MinHealth, m_MaxHealth;
	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
};
}
