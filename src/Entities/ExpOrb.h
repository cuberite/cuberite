
#pragma once

#include "Entity.h"





class cExpOrb :
	public cEntity
{
	typedef cExpOrb super;
	
public:
	
	cExpOrb(double a_X, double a_Y, double a_Z, int a_Reward);
	cExpOrb(const Vector3d & a_Pos, int a_Reward);

	// Override functions
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void SpawnOn(cClientHandle & a_Client) override;

	// cExpOrb functions
	int GetReward(void) const { return m_Reward; }

protected:
	int m_Reward;
} ;