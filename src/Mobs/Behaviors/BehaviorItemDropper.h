#pragma once

// mobTodo a more generic, not chickenspecific dropper
#include "Behavior.h"

class cBehaviorItemDropper : cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);
	void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)  override;

private:
		cMonster * m_Parent;  // Our Parent
		int m_EggDropTimer;
};
