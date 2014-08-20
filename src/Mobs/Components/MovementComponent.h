#pragma once

class cNewMonster;
class cEntity;
class cChunk;

class cMovementComponent
{
protected:
	cNewMonster * m_Self;
public:
	cMovementComponent(cNewMonster * a_Entity);
	virtual ~cMovementComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
