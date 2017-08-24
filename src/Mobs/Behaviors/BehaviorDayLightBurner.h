#pragma once

// mobTodo I just need vector3d
#include "Behavior.h"
#include "../../World.h"

// fwds
class cMonster;
class cEntity;
class cChunk;

class cBehaviorDayLightBurner : cBehavior
{
public:
	cBehaviorDayLightBurner(cMonster * a_Parent);

	void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	bool WouldBurnAt(Vector3d a_Location, cChunk & a_Chunk);

	// Functions our host Monster should invoke:
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);

private:
	cMonster * m_Parent;  // Our Parent
	cEntity * m_Attacker;  // The entity we're running away from
};
