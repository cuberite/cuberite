
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
			if (((cPlayer *)m_Target)->IsGameModeCreative())
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
	if (!((cPlayer *)a_Entity)->IsGameModeCreative())
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
	Vector3d AttackDirection(m_Target->GetPosition() - GetPosition());

	// TODO fix line of sight, or come to the conclusion that it isn't really needed here.
	// By temporarily disabling it we're fixing non exploding creepers.
	// Also, it isn't always detecting the wall and cMonster can attack through walls, I decreased their range in pull #2037 to resolve this temporarily.
	// My guess is that the line of sight trace fails when the zombie is partially in the wall.
	// Regarding creeper explosions: My guess is that the creeper's line of sight hits the block it's standing on.
	// Also, is LineOfSight.Trace inefficient? We're not feeding it our cChunk so it's probably getting one in some inefficient way.
	// if (ReachedFinalDestination() && !LineOfSight.Trace(GetPosition(), AttackDirection, (int)AttackDirection.Length()))
	if (ReachedFinalDestination())
	{
		// Attack if reached destination, target isn't null, and have a clear line of sight to target (so won't attack through walls)
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




bool cAggressiveMonster::IsMovingToTargetPosition()
{
	// Difference between destination x and target x is negligible (to 10^-12 precision)
	if (fabsf((float)m_FinalDestination.x - (float)m_Target->GetPosX()) < std::numeric_limits<float>::epsilon())
	{
		return false;
	}
	// Difference between destination z and target z is negligible (to 10^-12 precision)
	else if (fabsf((float)m_FinalDestination.z - (float)m_Target->GetPosZ()) > std::numeric_limits<float>::epsilon())
	{
		return false;
	}
	return true;
}
