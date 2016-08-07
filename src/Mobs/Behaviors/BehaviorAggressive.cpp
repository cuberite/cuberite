#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAggressive.h"
#include "BehaviorChaser.h"
#include "../Monster.h"
#include "../../Chunk.h"
#include "../../Entities/Player.h"



cBehaviorAggressive::cBehaviorAggressive(cMonster * a_Parent) : m_Parent(a_Parent)
{
	ASSERT(m_Parent != nullptr);
	ASSERT(GetTarget() != nullptr);
	m_ParentAttacker = m_Parent->GetBehaviorChaser();
}





bool cBehaviorAggressive::ActiveTick()
{
	// Target something new if we have no target
	if (GetTarget() == nullptr)
	{
		SetTarget(FindNewTarget());
		if (GetTarget() == nullptr)
		{
			return false;
		}
	}


	m_ParentAttacker->Attack(m_Target);

	#include "../../Tracer.h"
	cTracer LineOfSight(m_Parent->GetWorld());
	Vector3d MyHeadPosition = m_Parent->GetPosition() + Vector3d(0, m_Parent->GetHeight(), 0);
	Vector3d AttackDirection(GetTarget()->GetPosition() + Vector3d(0, GetTarget()->GetHeight(), 0) - MyHeadPosition);


	if (GetTarget() != nullptr)
	{
		MoveToPosition(GetTarget()->GetPosition());
	}
	if (TargetIsInRange() && !LineOfSight.Trace(MyHeadPosition, AttackDirection, static_cast<int>(AttackDirection.Length())) && (GetHealth() > 0.0))
	{
		// Attack if reached destination, target isn't null, and have a clear line of sight to target (so won't attack through walls)
		Attack(a_Dt);
	}
}





void cBehaviorAggressive::Destroyed()
{
	m_Target = nullptr;
}





bool cBehaviorAggressive::FindNewTarget()
{
	cPlayer * Closest = m_Parent->GetNearestPlayer();
	return Closest;  // May be null
}
