
// WitherSkullEntity.h

// Declares the cWitherSkullEntity class representing the entity used by both blue and black wither skulls





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cWitherSkullEntity :
public cProjectileEntity
{
	typedef cProjectileEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cWitherSkullEntity)

	cWitherSkullEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;

} ;  // tolua_export




