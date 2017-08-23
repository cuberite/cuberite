#pragma once

// Makes the mob follow specific held items
class cBehaviorItemFollower;

#include "Behavior.h"

// fwds
class cMonster;
class cItems;

class cBehaviorItemFollower : public cBehavior
{
public:
	cBehaviorItemFollower(cMonster * a_Parent);

	void GetBreedingItems(cItems & a_Items);

	// Functions our host Monster should invoke:
	bool IsControlDesired() override;
	void Tick() override;


private:
	/** Our parent */
	cMonster * m_Parent;
};
