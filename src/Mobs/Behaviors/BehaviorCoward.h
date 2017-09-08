#pragma once

#include "Behavior.h"

/** Makes the mob run away from any other mob that damages it.

Connections to other behaviors:
 - The mob should also have a cBehaviorAttacker, otherwise this behavior will not work.
 - This behavior does not make sense in combination with BehaviorAggressive or cBehaviorBrave.

Special connections:
 - None.

*/
class cBehaviorCoward : cBehavior
{
public:
	cBehaviorCoward();
	void AttachToMonster(cMonster & a_Parent);

	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	bool ControlStarting(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	bool ControlEnding(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;


private:
	cMonster * m_Parent;  // Our Parent
	cEntity * m_Attacker;  // The entity we're running away from
};
