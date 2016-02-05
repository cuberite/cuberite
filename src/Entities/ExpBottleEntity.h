
// ExpBottleEntity.h

// Declares the cExpBottleEntity class representing the thrown exp bottle





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cExpBottleEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cExpBottleEntity)

	cExpBottleEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;

	/** Breaks the bottle, fires its particle effects and sounds
	@param a_HitPos     The position where the bottle will break */
	void Break(const Vector3d & a_HitPos);

};  // tolua_export




