#pragma once



class cBehaviorDoNothing;

#include "Behavior.h"

/**  Always takes control of the tick and does nothing. Used for unimplemented mobs like squids. */
class cBehaviorDoNothing : public cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);
	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);

private:
	/** Our parent */
	cMonster * m_Parent;
};
