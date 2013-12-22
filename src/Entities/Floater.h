
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

	bool CanPickup(void)       const { return m_CanPickupItem; }
	int GetOwnerID(void)       const { return m_PlayerID; }
	int GetAttachedMobID(void) const { return m_AttachedMobID; }

protected:
	// Position
	Vector3d m_ParticlePos;

	// Bool needed to check if you can get a fish.
	bool m_CanPickupItem;

	// Countdown times
	int m_PickupCountDown;
	int m_CountDownTime;

	// Entity IDs
	int m_PlayerID;
	int m_AttachedMobID;
} ;