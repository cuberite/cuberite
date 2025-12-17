
#pragma once

#include "Globals.h"
#include "Item.h"
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
	cItems m_Items;
	double m_Range;
};
}
