#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttackerRanged.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../BlockID.h"
#include "../../Entities/ArrowEntity.h"

cBehaviorAttackerRanged::cBehaviorAttackerRanged(
	RangedShootingFunction a_RangedShootingFunction, int a_ProjectileAmount,
	int a_ShootingIntervals) :
	m_ShootingFunction(a_RangedShootingFunction),
	m_ProjectileAmount(a_ProjectileAmount),
	m_ShootingIntervals(a_ShootingIntervals)
{

}


bool cBehaviorAttackerRanged::DoStrike(int a_StrikeTickCnt)
{
	UNUSED(a_StrikeTickCnt);

	// stop shooting if target is lost
	if ((GetTarget() == nullptr))
	{
		return true;
	}

	// Stop shooting if we've shot m_ProjectileAmount times.
	if (a_StrikeTickCnt - 1 == m_ShootingIntervals * m_ProjectileAmount)
	{
		return true;
	}

	// shoot once every m_ShootingIntervals.
	// Starting immediately at first call to DoStrike
	if ((a_StrikeTickCnt - 1) % m_ShootingIntervals == 0)
	{
		m_ShootingFunction(*this, *m_Parent, *GetTarget());
	}
	return false;
}
