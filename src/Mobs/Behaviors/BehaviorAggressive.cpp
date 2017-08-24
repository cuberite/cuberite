#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAggressive.h"
#include "BehaviorChaser.h"
#include "../Monster.h"
#include "../../Chunk.h"
#include "../../Entities/Player.h"

void cBehaviorAggressive::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachPreTickBehavior(this);
}


void cBehaviorAggressive::PreTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	// Target something new if we have no target
	if (m_ParentChaser->GetTarget() == nullptr)
	{
		m_ParentChaser->SetTarget(FindNewTarget());
	}
}





cPawn * cBehaviorAggressive::FindNewTarget()
{
	cPlayer * Closest = m_Parent->GetNearestPlayer();
	return Closest;  // May be null
}
