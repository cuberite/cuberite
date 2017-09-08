#pragma once


class cBehaviorAggressive;

#include "Behavior.h"
#include <functional>

/** The mob is agressive toward specific mobtypes (mobTodo), or toward the player.
 *

Connections to other behaviors:
 - The mob should also have a cBehaviorAttacker, otherwise this behavior will not work.
 - This behavior does not make sense in combination with BehaviorCoward or cBehaviorBrave.

Special connections:
 - Relies on cMonster::GetAttackerBehavior to obtain the attacker behavior and alter its target.

*/

typedef std::function<bool(cBehaviorAggressive & a_Behavior, cMonster & a_Monster, cChunk & a_Chunk)> ShouldBeAggressiveFunction;

class cBehaviorAggressive : public cBehavior
{

public:
	cBehaviorAggressive(ShouldBeAggressiveFunction a_ShouldBeAggressiveFunction = nullptr);
	void AttachToMonster(cMonster & a_Parent);

	// cBehaviorAggressive(cMonster * a_Parent, bool a_HatesPlayer);
	// TODO agression toward specific players, and specific mobtypes, etc
	// Agression under specific conditions (nighttime, etc)

	// Functions our host Monster should invoke:
	void PreTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

private:
	cPawn * FindNewTarget();

	// Our parent
	cMonster * m_Parent;

	// The mob we want to attack
	cPawn * m_Target;

	ShouldBeAggressiveFunction m_ShouldBeAggressiveFunction;
	bool m_ShouldBeAgressive;
	int m_AgressionCheckCountdown;
};
