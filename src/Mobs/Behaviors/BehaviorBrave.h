#pragma once

#include "Behavior.h"

/** Makes the mob fight back any other mob that damages it. Mob should have BehaviorAttacker to work.
This behavior does not make sense in combination with BehaviorCoward. */
class cBehaviorBrave : cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

private:
	cMonster * m_Parent;  // Our Parent
};
