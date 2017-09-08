
// GhastFireballEntity.h

// Declares the cGhastFireballEntity class representing the ghast fireball in flight.





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cGhastFireballEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cGhastFireballEntity)

	cGhastFireballEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);

protected:

	void Explode(Vector3i a_Block);

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;

	// TODO: Deflecting the fireballs by arrow- or sword- hits

} ;  // tolua_export




