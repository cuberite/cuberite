#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAggressive.h"
#include "BehaviorChaser.h"
#include "../Monster.h"
#include "../../Chunk.h"
#include "../../Entities/Player.h"



cBehaviorAggressive::cBehaviorAggressive(cMonster * a_Parent) : m_Parent(a_Parent)
{
	ASSERT(m_Parent != nullptr);
	m_ParentChaser = m_Parent->GetBehaviorChaser();
	ASSERT(m_ParentChaser != nullptr);
}





void cBehaviorAggressive::PreTick()
{
	// Target something new if we have no target
	if (m_ParentChaser->GetTarget() == nullptr)
	{
		m_ParentChaser->SetTarget(FindNewTarget());
	}
}





void cBehaviorAggressive::Destroyed()
{
	m_ParentChaser->SetTarget(nullptr);
}





cPawn * cBehaviorAggressive::FindNewTarget()
{
	cPlayer * Closest = m_Parent->GetNearestPlayer();
	return Closest;  // May be null
}
