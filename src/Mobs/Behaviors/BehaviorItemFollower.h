#pragma once

// Makes the mob follow specific held items
class cBehaviorItemFollower;

#include "Behavior.h"

// fwds
class cMonster;
class cItems;

class cBehaviorItemFollower
{
public:
	cBehaviorItemFollower(cMonster * a_Parent);

	void GetBreedingItems(cItems & a_Items);

	// Functions our host Monster should invoke:
	void Tick();


private:
	/** Our parent */
	cMonster * m_Parent;
};
