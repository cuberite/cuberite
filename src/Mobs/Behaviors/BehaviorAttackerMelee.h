#pragma once

#include "BehaviorAttacker.h"
#include <functional>
class cBehaviorAttackerMelee;

/** Grants the mob that ability to approach a target and then melee attack it.
Use BehaviorAttackerMelee::SetTarget to attack.


Connections to other behaviors:
 - See cBehaviorAttacker.

Special connections:
 - None.

*/
typedef std::function<void(cBehaviorAttackerMelee & a_Behavior, cMonster & a_Attacker, cPawn & a_Attacked)> PostAttackFunction;
class cBehaviorAttackerMelee : public cBehaviorAttacker
{
public:
	cBehaviorAttackerMelee(PostAttackFunction a_PostAttackFunction = nullptr);
	bool DoStrike(int a_StrikeTickCnt) override;
private:
	 PostAttackFunction m_PostAttackFunction;
};
