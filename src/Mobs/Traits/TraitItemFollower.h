#pragma once

class cTraitItemFollower;
class iTraitItemFollower;

//fwds
class cMonster;
class cItems;





class iTraitItemFollower
{
public:
	virtual void GetFollowedItems(cItems & a_Items) const = 0;
	virtual ~iTraitItemFollower();
};





class cTraitItemFollower
{
public:
	cTraitItemFollower(iTraitItemFollower * a_Parent);

	// Functions our host Monster should invoke:
	bool ActiveTick();


private:
	/** Our parent */
	iTraitItemFollower * m_ParentInterface;
	cMonster * m_Parent;
};
