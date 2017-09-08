#pragma once

#include "Behavior.h"

/** Makes the mob fight back any other mob that damages it. Unlike cBehaviorAggressive, it will
not attack unless provoked.

Connections to other behaviors:
 - The mob should also have a cBehaviorAttacker, otherwise this behavior will not work.
 - This behavior does not make sense in combination with BehaviorCoward or cBehaviorAggressive.

Special connections:
 - Relies on cMonster::GetAttackerBehavior to obtain the attacker behavior and alter its target.

*/
class cBehaviorBrave : cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

private:
	cMonster * m_Parent;  // Our Parent
};
