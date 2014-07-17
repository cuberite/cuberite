
#pragma once

#include "Entity.h"





// tolua_begin
class cFloater :
	public cEntity
{
	typedef cEntity super;
	
public:
	// tolua_end

	CLASS_PROTODEF(cFloater);

	cFloater(double a_X, double a_Y, double a_Z, Vector3d a_Speed, int a_PlayerID, int a_CountDownTime);

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
	// tolua_begin
	bool CanPickup(void)       const { return m_CanPickupItem; }
	int GetOwnerID(void)       const { return m_PlayerID; }
	int GetAttachedMobID(void) const { return m_AttachedMobID; }
	// tolua_end

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
} ;  // tolua_export
