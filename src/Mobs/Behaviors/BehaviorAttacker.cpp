
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttacker.h"
#include "BehaviorStriker.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../Entities/Player.h"
#include "../../Tracer.h"


cBehaviorAttacker::cBehaviorAttacker() :
	m_AttackRate(3)
  , m_AttackDamage(1)
  , m_AttackRange(1)
  , m_AttackCoolDownTicksLeft(0)
  , m_TicksSinceLastDamaged(100)
  , m_IsStriking(false)
{

}





void cBehaviorAttacker::AttachToMonster(cMonster & a_Parent, cBehaviorStriker & a_ParentStriker)
{
	m_Parent = &a_Parent;
	m_Parent->AttachTickBehavior(this);
	m_Parent->AttachDestroyBehavior(this);
	m_Parent->AttachPostTickBehavior(this);
	m_Parent->AttachDoTakeDamageBehavior(this);
}





bool cBehaviorAttacker::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	// If we have a target, we have something to do! Return true and control the mob Ticks.
	// Otherwise return false.
	return (GetTarget() != nullptr);
}





void cBehaviorAttacker::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);

	if (m_IsStriking) return;
	// If we're striking, return. This allows derived classes to implement multi-tick strikes
	// E.g. a blaze shooting 3 fireballs in consequative ticks.
	// Derived class is expected to set m_IsStriking to false when the strike is done.

	if ((GetTarget() != nullptr))
	{
		ASSERT(GetTarget()->IsTicking());

		if (GetTarget()->IsPlayer())
		{
			if (!static_cast<cPlayer *>(GetTarget())->CanMobsTarget())
			{
				SetTarget(nullptr);
			}
		}
	}


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
			if (TargetIsInStrikeRadiusAndLineOfSight())
			{
				StrikeTargetIfReady();
			}
			else
			{
				m_Parent->MoveToPosition(m_Target->GetPosition());
				// todo BehaviorApproacher
			}
		}
	}
}





void cBehaviorAttacker::PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
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





void cBehaviorAttacker::DoTakeDamage(TakeDamageInfo & a_TDI)
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





void cBehaviorAttacker::Destroyed()
{
	SetTarget(nullptr);
}





void cBehaviorAttacker::SetAttackRate(float a_AttackRate)
{
	m_AttackRate = a_AttackRate;
}





void cBehaviorAttacker::SetAttackRange(int a_AttackRange)
{
	m_AttackRange = a_AttackRange;
}





void cBehaviorAttacker::SetAttackDamage(int a_AttackDamage)
{
	m_AttackDamage = a_AttackDamage;
}




cPawn * cBehaviorAttacker::GetTarget()
{
	return m_Target;
}





void cBehaviorAttacker::SetTarget(cPawn * a_Target)
{
	m_Target = a_Target;
}





bool cBehaviorAttacker::TargetIsInStrikeRadius(void)
{
	ASSERT(GetTarget() != nullptr);
	return ((GetTarget()->GetPosition() - m_Parent->GetPosition()).SqrLength() < (m_AttackRange * m_AttackRange));
}





bool cBehaviorAttacker::TargetIsInStrikeRadiusAndLineOfSight()
{
	ASSERT(m_Target != nullptr);
	ASSERT(m_Parent != nullptr);


	cTracer LineOfSight(m_Parent->GetWorld());
	Vector3d MyHeadPosition = m_Parent->GetPosition() + Vector3d(0, m_Parent->GetHeight(), 0);
	Vector3d AttackDirection(GetTarget()->GetPosition() + Vector3d(0, GetTarget()->GetHeight(), 0) - MyHeadPosition);

	if (TargetIsInStrikeRadius() &&
			!LineOfSight.Trace(MyHeadPosition, AttackDirection, static_cast<int>(AttackDirection.Length())) &&
			(m_Parent->GetHealth() > 0.0)
	)
	{
		return true;
	}
	else
	{
		return false;
	}
}





bool cBehaviorAttacker::TargetOutOfSight()
{
	ASSERT(m_Target != nullptr);
	if ((GetTarget()->GetPosition() - m_Parent->GetPosition()).Length() > m_Parent->GetSightDistance())
	{
		return true;
	}
	return false;
}





void cBehaviorAttacker::ResetStrikeCooldown()
{
	m_AttackCoolDownTicksLeft = static_cast<int>(3 * 20 * m_AttackRate);  // A second has 20 ticks, an attack rate of 1 means 1 hit every 3 seconds
}





void cBehaviorAttacker::StrikeTargetIfReady()
{
	if (m_AttackCoolDownTicksLeft != 0)
	{
		m_IsStriking = true;
		StrikeTarget(); // Different derived classes implement strikes in different ways
		ResetStrikeCooldown();
	}
}
