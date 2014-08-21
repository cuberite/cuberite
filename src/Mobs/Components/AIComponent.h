#pragma once

class cMonster;
class cEntity;
class cChunk;

class cAIComponent
{
protected:
	cMonster * m_Self;
public:
	cAIComponent(cMonster * a_Entity);
	virtual ~cAIComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
