#pragma once

class cNewMonster;
class cEntity;
class cChunk;

class cAttackComponent
{
protected:
	cNewMonster * m_Self;
public:
	cAttackComponent(cNewMonster * a_Entity);
	virtual ~cAttackComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
