#pragma once

class cMonster;
class cEntity;
class cChunk;

class cEnvironmentComponent
{
protected:
	cMonster * m_Self;
public:
	cEnvironmentComponent(cMonster * a_Entity);
	virtual ~cEnvironmentComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
