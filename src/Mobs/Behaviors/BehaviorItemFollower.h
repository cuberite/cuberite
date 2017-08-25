#pragma once

// Makes the mob follow specific held items
class cBehaviorItemFollower;

#include "Behavior.h"

class cBehaviorItemFollower : public cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);




private:
	/** Our parent */
	cMonster * m_Parent;
};
