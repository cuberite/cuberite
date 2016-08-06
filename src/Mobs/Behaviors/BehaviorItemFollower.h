#pragma once

class cBehaviorItemFollower;
class iBehaviorItemFollower;

//fwds
class cMonster;
class cItems;





class iBehaviorItemFollower
{
public:
	virtual void GetFollowedItems(cItems & a_Items) const = 0;
	virtual ~iBehaviorItemFollower();
};





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
