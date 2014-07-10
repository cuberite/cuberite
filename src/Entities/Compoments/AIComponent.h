#pragma once
#include "../Entity.h"
#include "../../Mobs/Monster.h"

class cAIComponent
{
protected:
	cMonster * m_Self;
public:
	cAIComponent(cEntity * a_Entity) : m_Self(a_Entity){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
};
