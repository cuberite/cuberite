#pragma once


class cBehaviorItemFollower;

#include "Behavior.h"
/** Makes the mob follow specific items when held by the player.
Currently relies on cMonster::GetFollowedItems for the item list. */
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
