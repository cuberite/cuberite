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
	void AttachToMonster(cMonster & a_Parent) override;

	void GetBreedingItems(cItems & a_Items);

	// Functions our host Monster should invoke:
	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;


private:
	/** Our parent */
	cMonster * m_Parent;
};
