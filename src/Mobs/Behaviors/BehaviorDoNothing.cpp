#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorDoNothing.h"
#include "../Monster.h"

void cBehaviorDoNothing::AttachToMonster(cMonster & a_Parent)
{
		m_Parent = &a_Parent;
		m_Parent->AttachTickBehavior(this);
}

bool cBehaviorDoNothing::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	return true;
}

void cBehaviorDoNothing::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	return;
}
