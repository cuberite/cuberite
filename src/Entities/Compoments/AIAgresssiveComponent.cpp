#include "AIAgressiveComponent.h"



void cAIAggressiveComponent::Tick(float a_Dt, cChunk & a_Chunk)
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

	if (m_Target == NULL)
		return;

	cTracer LineOfSight(GetWorld());
	Vector3d AttackDirection(m_Target->GetPosition() - GetPosition());

	if (ReachedFinalDestination() && !LineOfSight.Trace(GetPosition(), AttackDirection, (int)AttackDirection.Length()))
	{
		// Attack if reached destination, target isn't null, and have a clear line of sight to target (so won't attack through walls)
		Attack(a_Dt / 1000);
	}
}


void cAIAggressiveComponent::Attack(float a_Dt)
{
	float attack_interval = m_Self->GetAttackInterval();
	attack_interval += a_Dt * m_Self->GetAttackRate();

	if ((m_Target != NULL) && (attack_interval > 3.0))
	{
		// Setting this higher gives us more wiggle room for attackrate
		attack_interval = 0.0f;
		m_Target->TakeDamage(dtMobAttack, m_Self, m_AttackDamage, 0);
	}

	m_Self->SetAttackInterval(attack_interval)
}


bool cAIAggressiveComponent::IsMovingToTargetPosition()
{
	// Difference between destination x and target x is negligible (to 10^-12 precision)
	if (fabsf((float)m_Self->m_FinalDestination.x - (float)m_Target->GetPosX()) < std::numeric_limits<float>::epsilon())
	{
		return false;
	}
	// Difference between destination z and target z is negligible (to 10^-12 precision)
	else if (fabsf((float)m_Self->m_FinalDestination.z - (float)m_Target->GetPosZ()) > std::numeric_limits<float>::epsilon())
	{
		return false;
	}
	return true;
}


/// Event Checkers
void cAIAggressiveComponent::CheckEventLostPlayer(void)
{	
	if (m_Target != NULL)
	{
		if ((m_Target->GetPosition() - GetPosition()).Length() > m_Self->GetSightDistance())
		{
			EventLosePlayer();
		}
	}
	else
	{
		EventLosePlayer();
	}
}


/// Event Handlers
void cAggressiveMonster::EventSeePlayer(cEntity * a_Entity)
{
	if (!((cPlayer *)a_Entity)->IsGameModeCreative())
	{
		m_Target = a_Entity;
		m_EMState = CHASING;
	}
}


void cAIAggressiveComponent::EventLosePlayer(void)
{
	m_Target = NULL;
	m_EMState = IDLE;
}


/// State Logic
void cAggressiveMonster::InStateChasing(float a_Dt)
{
	super::InStateChasing(a_Dt);

	if (m_Target != NULL)
	{
		if (m_Target->IsPlayer())
		{
			if (((cPlayer *)m_Target)->IsGameModeCreative())
			{
				m_EMState = IDLE;
				return;
			}
		}

		if (!IsMovingToTargetPosition())
		{
			m_Self->MoveToPosition(m_Target->GetPosition());
		}
	}
} 


