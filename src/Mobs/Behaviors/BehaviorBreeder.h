#pragma once

// Grants breeding capabilities to the mob

class cBehaviorBreeder;

class cWorld;
class cMonster;
class cPlayer;
class cItems;





class cBehaviorBreeder
{

public:
    cBehaviorBreeder(cMonster * a_Parent, cItems & a_BreedingItems);

    // Functions our host Monster should invoke:
    void Tick();
    bool ActiveTick();
    void OnRightClicked(cPlayer & a_Player);
    void Destroyed();

    /** Returns the partner which the monster is currently mating with. */
    cMonster * GetPartner(void) const { return m_LovePartner; }

    /** Start the mating process. Causes the monster to keep bumping into the partner until m_MatingTimer reaches zero. */
    void EngageLoveMode(cMonster * a_Partner);

    /** Finish the mating process. Called after a baby is born. Resets all breeding related timers and sets m_LoveCooldown to 20 minutes. */
    void ResetLoveMode();

    /** Returns whether the monster has just been fed and is ready to mate. If this is "true" and GetPartner isn't "nullptr", then the monster is mating. */
    bool IsInLove() const;

    /** Returns whether the monster is tired of breeding and is in the cooldown state. */
    bool IsInLoveCooldown() const;

private:
    /** Our parent */
    cMonster * m_Parent;

    /** The monster's breeding partner. */
    cMonster * m_LovePartner;

    /** If above 0, the monster is in love mode, and will breed if a nearby monster is also in love mode. Decrements by 1 per tick till reaching zero. */
    int m_LoveTimer;

    /** If above 0, the monster is in cooldown mode and will refuse to breed. Decrements by 1 per tick till reaching zero. */
    int m_LoveCooldown;

    /** The monster is engaged in mating, once this reaches zero, a baby will be born. Decrements by 1 per tick till reaching zero, then a baby is made and ResetLoveMode() is called. */
    int m_MatingTimer;
};
