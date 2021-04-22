
// ThrownEggEntity.h

// Declares the cThrownEggEntity class representing a regular thrown egg





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownEggEntity :
	public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cThrownEggEntity)

	cThrownEggEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

private:

	/** Randomly decides whether to spawn a chicken where the egg lands. */
	void TrySpawnChicken(Vector3d a_HitPos);

	// cProjectileEntity overrides:
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;

} ;  // tolua_export
