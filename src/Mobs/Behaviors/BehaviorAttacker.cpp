
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttacker.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../Entities/Player.h"
#include "../../Tracer.h"


cBehaviorAttacker::cBehaviorAttacker() :
	m_AttackRate(3)
  , m_AttackDamage(1)
  , m_AttackRange(1)
  , m_AttackCoolDownTicksLeft(0)
  , m_IsStriking(false)
  , m_Target(nullptr)
  , m_ShouldRetaliate(true)
{

}





void cBehaviorAttacker::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachTickBehavior(this);
	m_Parent->AttachDestroyBehavior(this);
	m_Parent->AttachPostTickBehavior(this);
	m_Parent->AttachDoTakeDamageBehavior(this);
	m_Parent->m_BehaviorAttackerPointer = this;
}





bool cBehaviorAttacker::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	// If we have a target, we have something to do! Return true and control the mob Ticks.
	// Otherwise return false.
	return (m_IsStriking || (GetTarget() != nullptr));
}





void cBehaviorAttacker::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);

	ASSERT(m_Parent->GetHealth() > 0.0);

	if (m_IsStriking)
	{
		if (DoStrike(++m_StrikeTickCnt))
		{
			m_Parent->UnpinBehavior(this);
			m_IsStriking = false;
			ResetStrikeCooldown();
		}
		#ifdef _DEBUG
		if (m_StrikeTickCnt > 100)
		{
			LOGD("Sanity check failed. An attack took more than 5 seconds. Hmm");
			ASSERT(1 == 0);
		}
		#endif
		return;
	}

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
	if (GetTarget() != nullptr)
	{
		m_Parent->SetLookingAt(m_Target);
		if (TargetTooFar())
		{
			SetTarget(nullptr);
		}
		else
		{
			if (TargetIsInStrikeRadius() && TargetIsInLineOfSight())
			{
				StrikeIfReady();
			}
			else
			{
				m_Parent->MoveToPosition(m_Target->GetPosition());
				// todo BehaviorApproacher for creeper sneaking, etc
			}
		}
	}
}





void cBehaviorAttacker::PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_AttackCoolDownTicksLeft > 0)
	{
		m_AttackCoolDownTicksLeft -= 1;
	}
}





void cBehaviorAttacker::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!m_ShouldRetaliate)
	{
		return;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPawn())
	{
		if (
			(!a_TDI.Attacker->IsPlayer()) ||
			(static_cast<cPlayer *>(a_TDI.Attacker)->CanMobsTarget())
		)
		{
			SetTarget(static_cast<cPawn*>(a_TDI.Attacker));
		}
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





void cBehaviorAttacker::Strike()
{
	if (m_IsStriking)
	{
		return;
	}
	m_IsStriking = true;
	m_StrikeTickCnt = 0;
	m_Parent->PinBehavior(this);
	m_Parent->StopMovingToPosition();
}





bool cBehaviorAttacker::TargetIsInStrikeRadius(void)
{
	ASSERT(GetTarget() != nullptr);
	ASSERT(m_Parent != nullptr);
	return ((GetTarget()->GetPosition() - m_Parent->GetPosition()).SqrLength() < (m_AttackRange * m_AttackRange));
}





bool cBehaviorAttacker::TargetIsInLineOfSight()
{
	ASSERT(GetTarget() != nullptr);
	ASSERT(m_Parent != nullptr);


	// mobtodo make this smarter

	cTracer LineOfSight(m_Parent->GetWorld());
	Vector3d MyHeadPosition = m_Parent->GetHeadPosition();
	Vector3d TargetHeadPosition = GetTarget()->GetHeadPosition();
	Vector3d AttackDirection1(TargetHeadPosition - MyHeadPosition);

	if (LineOfSight.Trace(MyHeadPosition, AttackDirection1,
		static_cast<int>(AttackDirection1.Length()))
	)
	{
		return false;
	}

	Vector3d MyFeetPosition = m_Parent->GetPosition() + Vector3d(0, 0.5, 0);
	Vector3d TargetFeetPosition = GetTarget()->GetPosition() + Vector3d(0, 0.5, 0);
	Vector3d AttackDirection2(TargetFeetPosition - MyFeetPosition);
	if (LineOfSight.Trace(MyFeetPosition, AttackDirection2,
		static_cast<int>(AttackDirection2.Length()))
	)
	{
		return false;
	}

	return true;
}





bool cBehaviorAttacker::TargetTooFar()
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





void cBehaviorAttacker::StrikeIfReady()
{
	if (m_AttackCoolDownTicksLeft == 0)
	{
		Strike();
	}
}
