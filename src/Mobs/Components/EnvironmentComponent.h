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
public:
	cEnvironmentComponent(cMonster * a_Entity, int a_SightDistance);
	virtual ~cEnvironmentComponent(){}

	// Get Functions
	int GetSightDistance() { return m_SightDistance ; }
	bool GetOnGround() { return m_OnGround; }

	// Set Functions
	void SetSightDistance(int a_SightDistance) { m_SightDistance = a_SightDistance; }
	void SetOnGround(bool a_Bool) { m_OnGround = a_Bool; }
};
