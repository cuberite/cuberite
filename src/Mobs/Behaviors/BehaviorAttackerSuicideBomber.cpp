#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttackerSuicideBomber.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../BlockID.h"

bool cBehaviorAttackerSuicideBomber::StrikeTarget(int a_StrikeTickCnt)
{
	UNUSED(a_StrikeTickCnt);
	//mobTodo
	return true; // Finish the strike. It only takes 1 tick.
}

/*
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (((GetTarget() == nullptr) || !TargetIsInRange()) && !m_BurnedWithFlintAndSteel)
	{
		if (m_bIsBlowing)
		{
			m_ExplodingTimer = 0;
			m_bIsBlowing = false;
			m_World->BroadcastEntityMetadata(*this);
		}
	}
	else
	{
		if (m_bIsBlowing)
		{
			m_ExplodingTimer += 1;
		}

		if ((m_ExplodingTimer == 30) && (GetHealth() > 0.0))  // only explode when not already dead
		{
			m_World->DoExplosionAt((m_bIsCharged ? 5 : 3), GetPosX(), GetPosY(), GetPosZ(), false, esMonster, this);
			Destroy();  // Just in case we aren't killed by the explosion
		}
	}
 */
