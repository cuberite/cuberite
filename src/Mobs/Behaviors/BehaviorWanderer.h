#pragma once

// The mob will wander around

class cMonster;
class cEntity;
class cChunk;

class cBehaviorWanderer
{

public:
	cBehaviorWanderer(cMonster * a_Parent);

	// Functions our host Monster should invoke:
	bool ActiveTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);


private:
	cMonster * m_Parent;  // Our Parent

	std::chrono::milliseconds m_IdleInterval;
};
