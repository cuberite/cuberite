#pragma once

#include "BehaviorAttacker.h"
#include <functional>
class cBehaviorAttackerRanged;

/** Grants the mob that ability to approach a target and then melee attack it.
Use BehaviorAttackerMelee::SetTarget to attack. */
typedef std::function<void(cBehaviorAttackerRanged & a_Behavior,
	cMonster & a_Attacker, cPawn & a_Attacked)> RangedShootingFunction;

class cBehaviorAttackerRanged : public cBehaviorAttacker
{
public:
	cBehaviorAttackerRanged(RangedShootingFunction a_RangedShootingFUnction,
		int a_ProjectileAmount = 1, int a_ShootingIntervals = 1);
	bool DoStrike(int a_StrikeTickCnt) override;

private:
	RangedShootingFunction m_ShootingFunction;
	int m_ProjectileAmount;
	int m_ShootingIntervals;
};
