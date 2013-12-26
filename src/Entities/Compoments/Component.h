#pragma once

#include "Entity.h"

class cComponent
{
protected:
	cEntity * m_Self;
public:
	cComponent(cEntity * a_Entity) : m_Self(a_Entity){}


	// General
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL){}
	virtual void SpawnOn(cClientHandle & a_Client){}

	// World/Model Interaction
	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk){}
	virtual void OnCollisionWithEntity(cEntity * a_Entity){}

	// Environmental Reactions
	virtual void OnCollisionWithBlock(Vector3i & a_Position, BLOCKTYPE a_Block){}
	virtual void OnStartedBurning(){}
	virtual void OnFinishedBurning(){}

	// Reactions
	virtual void OnTakeDamage(TakeDamageInfo & a_TDI){}
	virtual void OnRightClicked(){}
	virtual void OnKilled(cEntity * a_Killer = NULL){}
	// virtual void OnPickup(){}
	// virtual void OnDestroy(){}

	// Logic
	virtual void OnAttackEntity(cEntity * a_Entity){}
};