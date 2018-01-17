
// ThrownEnderPearlEntity.h

// Declares the cThrownEnderPeralEntity class representing an ender pearl being thrown





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownEnderPearlEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cThrownEnderPearlEntity)

	cThrownEnderPearlEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	/** Teleports the creator where the ender pearl lands */
	void TeleportCreator(Vector3d a_HitPos);

private:

	/** Time in ticks to wait for the hit animation to begin before destroying */
	int m_DestroyTimer;

} ;  // tolua_export




