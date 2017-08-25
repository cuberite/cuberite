#pragma once

#include "Behavior.h"

// Makes the mob run away from any other mob that damages it


class cBehaviorCoward : cBehavior
{
public:
	cBehaviorCoward();
	void AttachToMonster(cMonster & a_Parent);

	// Functions our host Monster should invoke:
	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	bool ControlStarting(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	bool ControlEnding(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;


private:
	cMonster * m_Parent;  // Our Parent
	cEntity * m_Attacker;  // The entity we're running away from
};
