#pragma once

// mobTodo I just need vector3d
#include "../../World.h"

// fwds
class cMonster;
class cEntity;
class cChunk;

class cBehaviorDayLightBurner
{
public:
    cBehaviorDayLightBurner(cMonster * a_Parent);

    void Tick(cChunk & a_Chunk, bool WouldBurn);
    bool WouldBurnAt(Vector3d a_Location, cChunk & a_Chunk);

    // Functions our host Monster should invoke:
    void Tick();

private:
    cMonster * m_Parent;  // Our Parent
    cEntity * m_Attacker;  // The entity we're running away from
};
