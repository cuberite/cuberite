#pragma once

// Makes the mob follow specific held items

class cBehaviorItemFollower;
class iBehaviorItemFollower;

//fwds
class cMonster;
class cItems;





class cBehaviorItemFollower
{
public:
	cBehaviorItemFollower(iBehaviorItemFollower * a_Parent);

	// Functions our host Monster should invoke:
	bool ActiveTick();


private:
	/** Our parent */
	iBehaviorItemFollower * m_ParentInterface;
	cMonster * m_Parent;
};
