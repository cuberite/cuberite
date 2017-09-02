#pragma once

#include "BehaviorAttacker.h"

/** Grants the mob that ability to approach a target and then melee attack it.
Use BehaviorAttackerMelee::SetTarget to attack. */
class cBehaviorAttackerSuicideBomber : public cBehaviorAttacker
{
public:
	cBehaviorAttackerSuicideBomber();

	// cBehaviorAttacker also implements those and we need to call super on them
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

	bool DoStrike(int a_StrikeTickCnt) override;
	void OnRightClicked(cPlayer & a_Player) override;

	bool IsBlowing(void) const;
	bool IsCharged(void) const;
	bool IsBurnedWithFlintAndSteel(void) const;

private:
	bool m_bIsBlowing, m_bIsCharged, m_BurnedWithFlintAndSteel;
};
