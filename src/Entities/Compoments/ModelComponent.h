#pragma once

#include "../Entity.h"

class cModelComponent
{
protected:
	cEntity * m_Self;
public:
	cModelComponent(cEntity * a_Entity) : m_Self(a_Entity){}
	virtual void SpawnOn(cClientHandle & a_Client){}
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL){}

	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk){}
	virtual void OnCollisionWithEntity(cEntity * a_Entity){}


};
