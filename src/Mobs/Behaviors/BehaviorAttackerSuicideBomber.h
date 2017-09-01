#pragma once

#include "BehaviorAttacker.h"

/** Grants the mob that ability to approach a target and then melee attack it.
Use BehaviorAttackerMelee::SetTarget to attack. */
class cBehaviorAttackerSuicideBomber : public cBehaviorAttacker
{
public:
	bool StrikeTarget(int a_StrikeTickCnt) override;
	void OnRightClicked(cPlayer & a_Player) override;

private:

};
