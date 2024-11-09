
// WitherSkullEntity.h

// Declares the cWitherSkullEntity class representing the entity used by both blue and black wither skulls





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cWitherSkullEntity :
public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cWitherSkullEntity)

	cWitherSkullEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;

} ;  // tolua_export




