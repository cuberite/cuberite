// The mob is agressive toward specific mobtypes, or toward the player.
// This Behavior has a dependency on BehaviorChaser.
#pragma once
class cPawn;
class cMonster;
class cBehaviorChaser;

class cBehaviorAggressive
{

public:
    cBehaviorAggressive(cMonster * a_Parent);

    // cBehaviorAggressive(cMonster * a_Parent, bool a_HatesPlayer);
    // TODO agression toward specific players, and specific mobtypes, etc
    // Agression under specific conditions (nighttime, etc)

    // Functions our host Monster should invoke:
    bool ActiveTick();
    void Destroyed();

private:
    cPawn * FindNewTarget();

    // Our parent
    cMonster * m_Parent;
    cBehaviorChaser * m_ParentChaser;

    // The mob we want to attack
    cPawn * m_Target;
};
