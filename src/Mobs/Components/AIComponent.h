#pragma once

class cNewMonster;
class cEntity;
class cChunk;

class cAIComponent
{
protected:
	cNewMonster * m_Self;
public:
	cAIComponent(cNewMonster * a_Entity);
	virtual ~cAIComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
