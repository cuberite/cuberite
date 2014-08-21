#pragma once

class cMonster;
class cEntity;
class cChunk;

class cMovementComponent
{
protected:
	cMonster * m_Self;
public:
	cMovementComponent(cMonster * a_Entity);
	virtual ~cMovementComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
