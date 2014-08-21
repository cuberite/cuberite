#pragma once

class cMonster;
class cEntity;
class cChunk;

class cAttackComponent
{
protected:
	cMonster * m_Self;
public:
	cAttackComponent(cMonster * a_Entity);
	virtual ~cAttackComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
