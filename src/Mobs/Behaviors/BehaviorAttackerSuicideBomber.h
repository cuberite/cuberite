#pragma once

#include "BehaviorAttacker.h"

/** Grants the mob that ability to approach a target and then blow up.
Use cBehaviorAttackerSuicideBomber::SetTarget to attack.

Connections to other behaviors:
 - See cBehaviorAttacker.

Special connections:
 - The creeper will determine animation state by calling IsBlowing, IsCharged, and IsBurnedWithFlintAndSteel

*/
class cBehaviorAttackerSuicideBomber : public cBehaviorAttacker
{
public:
	/** a_ShouldBroadcastEntityMetadata should only be true for creepers or
	any future mobs that animate explosions. */
	cBehaviorAttackerSuicideBomber(bool a_ShouldBroadcastEntityMetadata = false);
	void AttachToMonster(cMonster & a_Parent) override;
	// cBehaviorAttacker also implements those and we need to call super on them
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

	bool DoStrike(int a_StrikeTickCnt) override;
	void OnRightClicked(cPlayer & a_Player) override;

	bool IsBlowing(void) const;
	bool IsCharged(void) const;
	bool IsBurnedWithFlintAndSteel(void) const;

private:
	bool m_bIsBlowing, m_bIsCharged, m_BurnedWithFlintAndSteel;
	bool m_ShouldBroadcastEntityMetadata;
};
