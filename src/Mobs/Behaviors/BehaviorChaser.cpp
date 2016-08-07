
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorChaser.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "BehaviorStriker.h"


, m_AttackRate(3)
, m_AttackDamage(1)
, m_AttackRange(1)
, m_AttackCoolDownTicksLeft(0)

cBehaviorChaser::cBehaviorChaser(cMonster * a_Parent) :
	m_Parent(a_Parent)
{
	ASSERT(m_Parent != nullptr);
	m_StrikeBehavior = m_Parent->GetBehaviorStriker();
	ASSERT(m_StrikeBehavior != nullptr);  // The monster that has an Attacker behavior must also have a Striker behavior
}





void cBehaviorChaser::Chase()
{
	// Stop targeting out of range targets
	if (GetTarget() != nullptr)
	{
		if (TargetOutOfSight())
		{
			SetTarget(nullptr);
		}
	}

	if (GetTarget() != nullptr)
	{
		if (TargetIsInStrikeRange())
		{
			StrikeTarget();
		}
		else
		{
			ApproachTarget();
			// m_Parent->MoveToPosition(GetTarget()->GetPosition());
		}
	}
}





void cBehaviorChaser::Tick()
{

}





void cBehaviorChaser::Destroyed()
{
	m_Target = nullptr;
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





cBehaviorChaser::~cBehaviorChaser()
{

}





bool cBehaviorChaser::TargetIsInStrikeRange()
{
	ASSERT(m_Target != nullptr);
	ASSERT(m_Parent != nullptr);
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
		m_StrikeBehavior->Strike(m_Target);  // LogicParrot Todo animations (via counter passing?)
		ResetStrikeCooldown();
	}
}
