#pragma once

#include "../Entity.h"

class cEnvironmentComponent
{
protected:
	cEntity * m_Self;
public:
	cEnvironmentComponent(cEntity * a_Entity) : m_Self(a_Entity){}

	virtual void Tick(float a_Dt, cChunk & a_Chunk){}

	virtual void OnCollisionWithBlock(Vector3i & a_Position, BLOCKTYPE a_Block){}
	virtual void OnStartedBurning(){}
	virtual void OnFinishedBurning(){}
	virtual void OnStartedDrowning(){}
	virtual void OnFinishedDrowning(){}

};
