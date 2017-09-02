#pragma once

#include "BehaviorAttacker.h"

/** Grants the mob that ability to approach a target and then melee attack it.
Use BehaviorAttackerMelee::SetTarget to attack. */
class cBehaviorAttackerSuicideBomber : public cBehaviorAttacker
{
public:
	bool StrikeTarget(int a_StrikeTickCnt) override;
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void OnRightClicked(cPlayer & a_Player) override;

	bool IsBlowing(void) const {return m_bIsBlowing; }
	bool IsCharged(void) const {return m_bIsCharged; }
	bool IsBurnedWithFlintAndSteel(void) const {return m_BurnedWithFlintAndSteel; }

private:
	bool m_bIsBlowing, m_bIsCharged, m_BurnedWithFlintAndSteel;
	int m_ExplodingTimer;
};
