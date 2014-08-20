
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "AIAggressiveComponent.h"
#include "../Entities/Player.h"
#include "../Tracer.h"
#include <iostream>

#include "Monster.h"


cAIAggressiveComponent::cAIAggressiveComponent(cMonster * a_Monster) : cAIComponent(a_Monster), m_Target(NULL){}

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

	cTracer LineOfSight(m_Self->GetWorld());
	Vector3d AttackDirection(m_Target->GetPosition() - m_Self->GetPosition());

	if (ReachedFinalDestination() && !LineOfSight.Trace(m_Self->GetPosition(), AttackDirection, (int)AttackDirection.Length()))
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
		m_Target->TakeDamage(dtMobAttack, m_Self, m_Self->GetAttackDamage(), 0);
	}

	m_Self->SetAttackInterval(attack_interval);
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


bool cAIAggressiveComponent::ReachedFinalDestination()
{
	if ((m_Self->GetPosition() - m_Self->m_FinalDestination).Length() <= m_Self->GetAttackRange())
	{
		return true;
	}
	
	return false;
}


/// Event Checkers
//Checks to see if EventSeePlayer should be fired
//monster sez: Do I see the player
void cAIAggressiveComponent::CheckEventSeePlayer(void)
{
	// TODO: Rewrite this to use cWorld's DoWithPlayers()
	cPlayer * Closest = m_Self->GetWorld()->FindClosestPlayer(m_Self->GetPosition(), (float)m_Self->GetSightDistance(), false);

	if (Closest != NULL)
	{
		std::cout << "Found Player\n";
		EventSeePlayer(Closest);
	}
}


void cAIAggressiveComponent::CheckEventLostPlayer(void)
{	
	if (m_Target != NULL)
	{
		if ((m_Target->GetPosition() - m_Self->GetPosition()).Length() > m_Self->GetSightDistance())
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
void cAIAggressiveComponent::EventSeePlayer(cEntity * a_Entity)
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
void cAIAggressiveComponent::InStateChasing(float a_Dt)
{

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


