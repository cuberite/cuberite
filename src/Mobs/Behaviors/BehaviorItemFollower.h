#pragma once

// Makes the mob follow specific held items
class cBehaviorItemFollower;

#include "Behavior.h"

class cBehaviorItemFollower : public cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);
	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);

private:
	/** Our parent */
	cMonster * m_Parent;
};
