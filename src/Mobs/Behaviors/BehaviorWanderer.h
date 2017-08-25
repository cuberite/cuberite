#pragma once

// The mob will wander around
#include <chrono>
#include "Behavior.h"

class cBehaviorWanderer : cBehavior
{

public:
	cBehaviorWanderer();
	void AttachToMonster(cMonster & a_Parent);

	// Functions our host Monster should invoke:
	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;


private:
	cMonster * m_Parent;  // Our Parent

	std::chrono::milliseconds m_IdleInterval;
};
