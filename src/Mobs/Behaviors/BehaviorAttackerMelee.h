#pragma once

#include "BehaviorAttacker.h"

/** Makes the mob fight back any other mob that damages it. Mob should have BehaviorAttacker to work.
This behavior does not make sense in combination with BehaviorCoward. */
class cBehaviorAttackerMelee : public cBehaviorAttacker
{
public:
	bool StrikeTarget(int a_StrikeTickCnt) override;
};
