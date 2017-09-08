#pragma once

// mobTodo I just need vector3d
#include "Behavior.h"
#include "../../World.h"

/**
The mob will burn in daylight.

Connections to other behaviors:
 - None.

Special connections:
 - Alters the pathfinder, so that the mob will try avoiding sunlight.

*/
class cBehaviorDayLightBurner : cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);
	void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	static bool WouldBurnAt(Vector3d a_Location, cChunk & a_Chunk, cMonster & a_Monster);

private:
	cMonster * m_Parent;  // Our Parent
	cEntity * m_Attacker;  // The entity we're running away from
};
