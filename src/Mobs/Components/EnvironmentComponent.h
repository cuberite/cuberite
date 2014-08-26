#pragma once

class cMonster;
class cEntity;
class cChunk;

class cEnvironmentComponent
{
protected:
	cMonster * m_Self;
	int m_SightDistance;
	bool m_OnGround;

	bool m_BurnsInDaylight;
public:
	cEnvironmentComponent(cMonster * a_Entity, int a_SightDistance);
	virtual ~cEnvironmentComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk);

	// Get Functions
	int GetSightDistance() { return m_SightDistance ; }
	bool GetOnGround() { return m_OnGround; }
	bool GetBurnsInDaylight() { return m_BurnsInDaylight; }

	// Set Functions
	void SetSightDistance(int a_SightDistance) { m_SightDistance = a_SightDistance; }
	void SetOnGround(bool a_Bool) { m_OnGround = a_Bool; }
	void SetBurnsInDaylight(bool a_Bool) { m_BurnsInDaylight = a_Bool; }

	// Handle functions
	void HandleDaylightBurning(cChunk & a_Chunk);
};
