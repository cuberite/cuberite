
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "AggressiveMonster.h"

#include "../World.h"
#include "../Entities/Player.h"
#include "../Tracer.h"





cAggressiveMonster::cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height)
{
	m_EMPersonality = AGGRESSIVE;
}





// What to do if in Chasing State
void cAggressiveMonster::InStateChasing(std::chrono::milliseconds a_Dt)
{
	super::InStateChasing(a_Dt);

	if (m_Target != nullptr)
	{
		if (m_Target->IsPlayer())
		{
			if (static_cast<cPlayer *>(m_Target)->IsGameModeCreative())
			{
				m_EMState = IDLE;
				return;
			}
		}
		MoveToPosition(m_Target->GetPosition());
	}
}





void cAggressiveMonster::EventSeePlayer(cEntity * a_Entity)
{
	if (!static_cast<cPlayer *>(a_Entity)->IsGameModeCreative())
	{
		super::EventSeePlayer(a_Entity);
		m_EMState = CHASING;
	}
}





void cAggressiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_EMState == CHASING)
	{
		CheckEventLostPlayer();
	}
	else
	{
		CheckEventSeePlayer();
	}

	if (m_Target == nullptr)
	{
		return;
	}

	cTracer LineOfSight(GetWorld());
	Vector3d MyHeadPosition = GetPosition() + Vector3d(0, GetHeight(), 0);
	Vector3d AttackDirection(m_Target->GetPosition() + Vector3d(0, m_Target->GetHeight(), 0) - MyHeadPosition);


	if (TargetIsInRange() && !LineOfSight.Trace(MyHeadPosition, AttackDirection, static_cast<int>(AttackDirection.Length())))
	{
		// Attack if reached destination, target isn't null, and have a clear line of sight to target (so won't attack through walls)
		StopMovingToPosition();
		Attack(a_Dt);
	}
}





void cAggressiveMonster::Attack(std::chrono::milliseconds a_Dt)
{
	m_AttackInterval += (static_cast<float>(a_Dt.count()) / 1000) * m_AttackRate;
	if ((m_Target == nullptr) || (m_AttackInterval < 3.0))
	{
		return;
	}

	// Setting this higher gives us more wiggle room for attackrate
	m_AttackInterval = 0.0;
	m_Target->TakeDamage(dtMobAttack, this, m_AttackDamage, 0);
}
