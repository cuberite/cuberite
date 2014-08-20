#pragma once

class cNewMonster;
class cEntity;
class cChunk;

class cEnvironmentComponent
{
protected:
	cNewMonster * m_Self;
public:
	cEnvironmentComponent(cNewMonster * a_Entity);
	virtual ~cEnvironmentComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
