#pragma once

class cMonster;
class cEntity;
class cChunk;

class cEnvironmentComponent
{
protected:
	cMonster * m_Self;
	int m_SightDistance;
public:
	cEnvironmentComponent(cMonster * a_Entity);
	virtual ~cEnvironmentComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}

	// Get Functions
	int GetSightDistance() { return m_SightDistance ; }

	// Set Functions
	void SetSightDistance(int a_SightDistance) { m_SightDistance = a_SightDistance; }
};
