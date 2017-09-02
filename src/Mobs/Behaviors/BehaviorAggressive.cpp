#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAggressive.h"
#include "BehaviorAttacker.h"
#include "../Monster.h"
#include "../../Chunk.h"
#include "../../Entities/Player.h"



cBehaviorAggressive::cBehaviorAggressive(ShouldBeAggressiveFunction a_ShouldBeAggressiveFunction)
	: m_ShouldBeAggressiveFunction(a_ShouldBeAggressiveFunction)
	, m_ShouldBeAgressive(true)
	, m_AgressionCheckCountdown(1)
{

}

void cBehaviorAggressive::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachPreTickBehavior(this);
}


void cBehaviorAggressive::PreTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);

	if (m_ShouldBeAggressiveFunction != nullptr)
	{
		if (--m_AgressionCheckCountdown == 0)
		{
			m_AgressionCheckCountdown = 40;
			m_ShouldBeAgressive = m_ShouldBeAggressiveFunction(*this, *m_Parent);
		}
	}

	if (!m_ShouldBeAgressive)
	{
		return;
	}

	// Target something new if we have no target
	cBehaviorAttacker * BehaviorAttacker = m_Parent->GetBehaviorAttacker();
	if ((BehaviorAttacker != nullptr) && (BehaviorAttacker->GetTarget() == nullptr))
	{
		// mobTodo enhance this
		BehaviorAttacker->SetTarget(FindNewTarget());
	}
}





cPawn * cBehaviorAggressive::FindNewTarget()
{
	cPlayer * Closest = m_Parent->GetNearestPlayer();
	if ((Closest != nullptr) && (!Closest->CanMobsTarget()))
	{
		return nullptr;
	}
	return Closest;  // May be null
}
