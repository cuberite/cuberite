
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

	cExpBottleEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, Vector3d a_Speed);

	cExpBottleEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, Vector3d a_HitPos) override;

	/** Breaks the bottle, fires its particle effects and sounds
	@param a_HitPos     The position where the bottle will break */
	void Break(Vector3d a_HitPos);

};  // tolua_export




