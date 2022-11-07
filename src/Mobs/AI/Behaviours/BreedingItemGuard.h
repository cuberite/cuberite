
#pragma once
#include "Mobs/AI/BehaviorTree.h"

/**
Checks for a player near to the entity holding the specified item.
Writes the position to the blackboard with the key FeederPosition.
*/

namespace BehaviorTree
{
class cBreedingItemGuard : public cTask
{
public:
	cBreedingItemGuard(cLuaState & a_LuaState);

	virtual eStatus DoTick(cBlackboard & a_Blackboard) override;
private:
	short m_Item;
	double m_Range;
};
}
