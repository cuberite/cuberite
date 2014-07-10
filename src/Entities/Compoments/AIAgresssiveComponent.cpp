#include "AIAgressiveComponent.h"

void cAIAggressiveComponent::Attack(float a_Dt)
{
	float attack_interval = m_Self->GetAttackInterval();
	attack_interval += a_Dt * m_Self->GetAttackRate();

	if ((m_Target != NULL) && (attack_interval > 3.0))
	{
		// Setting this higher gives us more wiggle room for attackrate
		attack_interval = 0.0f;
		m_Target->TakeDamage(dtMobAttack, this, m_AttackDamage, 0);
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