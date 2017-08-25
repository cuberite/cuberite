#pragma once


class cBehaviorAggressive;

#include "Behavior.h"
class cBehaviorChaser;

/** The mob is agressive toward specific mobtypes, or toward the player.
This Behavior has a dependency on BehaviorChaser. */
class cBehaviorAggressive : public cBehavior
{

public:
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
	cBehaviorChaser * m_ParentChaser;

	// The mob we want to attack
	cPawn * m_Target;
};
