#pragma once

#include "../Entity.h"

class cAIComponent
{
protected:
	cEntity * m_Self;
public:
	cAIComponent(cEntity * a_Entity) : m_Self(a_Entity){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}

};
