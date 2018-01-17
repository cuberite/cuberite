
// ThrownEggEntity.h

// Declares the cThrownEggEntity class representing a regular thrown egg





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownEggEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cThrownEggEntity)

	cThrownEggEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	// Randomly decides whether to spawn a chicken where the egg lands.
	void TrySpawnChicken(Vector3d a_HitPos);

private:

	/** Time in ticks to wait for the hit animation to begin before destroying */
	int m_DestroyTimer;

} ;  // tolua_export




