
#pragma once

#include "Entity.h"





// tolua_begin
class cFloater :
	public cEntity
{
	// tolua_end

	using Super = cEntity;

public:  // tolua_export

	CLASS_PROTODEF(cFloater)

	cFloater(Vector3d a_Pos, Vector3d a_Speed, UInt32 a_PlayerID, int a_CountDownTime);

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	// tolua_begin
	bool CanPickup(void)       const { return m_CanPickupItem; }
	UInt32 GetOwnerID(void)       const { return m_PlayerID; }
	UInt32 GetAttachedMobID(void) const { return m_AttachedMobID; }
	Vector3d GetBitePos(void)     const { return m_BitePos; }
	// tolua_end

protected:
	// Position
	Vector3d m_ParticlePos;

	// Position just before the floater gets pulled under by a fish
	Vector3d m_BitePos;

	// Bool needed to check if you can get a fish.
	bool m_CanPickupItem;

	// Countdown times
	int m_PickupCountDown;
	int m_CountDownTime;

	// Entity IDs
	UInt32 m_PlayerID;
	UInt32 m_AttachedMobID;
} ;  // tolua_export
