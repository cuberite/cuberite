
#pragma once

#include "Entity.h"





class cFloater :
	public cEntity
{
	typedef cFloater super;
	
public:
	
	cFloater(double a_X, double a_Y, double a_Z, Vector3d a_Speed, int a_PlayerID, int a_CountDownTime);

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	bool CanPickup(void) const { return m_CanPickupItem; }

protected:
	Vector3d m_Speed;
	Vector3d m_ParticlePos;
	int m_PickupCountDown;
	int m_PlayerID;
	int m_CountDownTime;
	bool m_CanPickupItem;
} ;