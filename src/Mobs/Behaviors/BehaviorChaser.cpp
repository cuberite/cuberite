
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorChaser.h"
#include "BehaviorStriker.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../Entities/Player.h"



cBehaviorChaser::cBehaviorChaser(cMonster * a_Parent) :
	m_Parent(a_Parent)
  , m_AttackRate(3)
  , m_AttackDamage(1)
  , m_AttackRange(1)
  , m_AttackCoolDownTicksLeft(0)
  , m_TicksSinceLastDamaged(100)
{
	ASSERT(m_Parent != nullptr);
}




bool cBehaviorChaser::IsControlDesired()
{
	// If we have a target, we have something to do! Return true and control the mob Ticks.
	// Otherwise return false.
	return (GetTarget() != nullptr);
}





void cBehaviorChaser::Tick()
{
	/*
	 * 	if ((GetTarget() != nullptr))
	{
		ASSERT(GetTarget()->IsTicking());

		if (GetTarget()->IsPlayer())
		{
			if (!static_cast<cPlayer *>(GetTarget())->CanMobsTarget())
			{
				SetTarget(nullptr);
			}
		}
	} //mobTodo copied from monster.cpp
	 * */

	ASSERT((GetTarget() == nullptr) || (GetTarget()->IsPawn() && (GetTarget()->GetWorld() == m_Parent->GetWorld())));
	// Stop targeting out of range targets
	if (GetTarget() != nullptr)
	{
		if (TargetOutOfSight())
		{
			SetTarget(nullptr);
		}
		else
		{
			if (TargetIsInStrikeRange())
			{
				StrikeTarget();
			}
			else
			{
				ApproachTarget(); // potential mobTodo: decoupling approaching from attacking
				// Not important now, but important for future extensibility, e.g.
				// cow chases wheat but using the netherman approacher to teleport around.
			}
		}
	}
}

void cBehaviorChaser::ApproachTarget()
{
	// potential mobTodo inheritence for creaper approachers, etc
	m_Parent->MoveToPosition(m_Target->GetPosition());
}



void cBehaviorChaser::PostTick()
{
	if (m_TicksSinceLastDamaged < 100)
	{
		++m_TicksSinceLastDamaged;
	}

	if (m_AttackCoolDownTicksLeft > 0)
	{
		m_AttackCoolDownTicksLeft -= 1;
	}
}





void cBehaviorChaser::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPawn())
	{
		if (
			(!a_TDI.Attacker->IsPlayer()) ||
			(static_cast<cPlayer *>(a_TDI.Attacker)->CanMobsTarget())
		)
		{
			SetTarget(static_cast<cPawn*>(a_TDI.Attacker));
		}
		m_TicksSinceLastDamaged = 0;
	}
}





void cBehaviorChaser::Destroyed()
{
	SetTarget(nullptr);
}





void cBehaviorChaser::SetAttackRate(float a_AttackRate)
{
	m_AttackRate = a_AttackRate;
}





void cBehaviorChaser::SetAttackRange(int a_AttackRange)
{
	m_AttackRange = a_AttackRange;
}





void cBehaviorChaser::SetAttackDamage(int a_AttackDamage)
{
	m_AttackDamage = a_AttackDamage;
}




cPawn * cBehaviorChaser::GetTarget()
{
	return m_Target;
}





void cBehaviorChaser::SetTarget(cPawn * a_Target)
{
	m_Target = a_Target;
}





bool cBehaviorChaser::TargetIsInStrikeRange()
{
	ASSERT(m_Target != nullptr);
	ASSERT(m_Parent != nullptr);
	/*
	#include "../../Tracer.h"
	cTracer LineOfSight(m_Parent->GetWorld());
	Vector3d MyHeadPosition = m_Parent->GetPosition() + Vector3d(0, m_Parent->GetHeight(), 0);
	Vector3d AttackDirection(m_ParentChaser->GetTarget()->GetPosition() + Vector3d(0, GetTarget()->GetHeight(), 0) - MyHeadPosition);


	if (GetTarget() != nullptr)
	{
		MoveToPosition(GetTarget()->GetPosition());
	}
	if (TargetIsInRange() && !LineOfSight.Trace(MyHeadPosition, AttackDirection, static_cast<int>(AttackDirection.Length())) && (GetHealth() > 0.0))
	{
		// Attack if reached destination, target isn't null, and have a clear line of sight to target (so won't attack through walls)
		Attack(a_Dt);
	}
	*/

	return ((m_Target->GetPosition() - m_Parent->GetPosition()).SqrLength() < (m_AttackRange * m_AttackRange));
}





bool cBehaviorChaser::TargetOutOfSight()
{
	ASSERT(m_Target != nullptr);
	if ((GetTarget()->GetPosition() - m_Parent->GetPosition()).Length() > m_Parent->GetSightDistance())
	{
		return true;
	}
	return false;
}





void cBehaviorChaser::ResetStrikeCooldown()
{
	m_AttackCoolDownTicksLeft = static_cast<int>(3 * 20 * m_AttackRate);  // A second has 20 ticks, an attack rate of 1 means 1 hit every 3 seconds
}





void cBehaviorChaser::StrikeTarget()
{
	if (m_AttackCoolDownTicksLeft != 0)
	{
		cBehaviorStriker * Striker = m_Parent->GetBehaviorStriker();
		if (Striker != nullptr)
		{
			// Striker->Strike(m_Target); //mobTodo
		}
		ResetStrikeCooldown();
	}
}
