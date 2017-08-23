#pragma once

#include "Behavior.h"

// Makes the mob run away from any other mob that damages it

//fwds
class cMonster;
class cItems;
class cEntity;
struct TakeDamageInfo;

class cBehaviorCoward
{
public:
	cBehaviorCoward(cMonster * a_Parent);

	// Functions our host Monster should invoke:
	bool Tick();
	void DoTakeDamage(TakeDamageInfo & a_TDI);


private:
	cMonster * m_Parent;  // Our Parent
	cEntity * m_Attacker;  // The entity we're running away from
};
