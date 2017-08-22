#pragma once

// fwds
class cMonster;
class cEntity;
class cChunk;
class Vector3d;

class cBehaviorDayLightBurner
{
    cBehaviorDayLightBurner(cMonster * a_Parent);

    bool WouldBurnAt(Vector3d & a_Location, cChunk & a_Chunk);

    // Functions our host Monster should invoke:
    void Tick();

private:
    cMonster * m_Parent;  // Our Parent
    cEntity * m_Attacker;  // The entity we're running away from
};
