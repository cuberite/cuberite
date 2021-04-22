
// FireChargeEntity.h

// Declares the cFireChargeEntity representing the fire charge shot by the blaze





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cFireChargeEntity :
	public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cFireChargeEntity)

	cFireChargeEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

protected:

	void Explode(Vector3i a_Block);

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;

} ;  // tolua_export




