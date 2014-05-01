//
//  ThrownEggEntity.h
//

#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownEggEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	
	// tolua_end
	
	CLASS_PROTODEF(cThrownEggEntity);
	
	cThrownEggEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);
	
protected:
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	
	// Randomly decides whether to spawn a chicken where the egg lands.
	void TrySpawnChicken(const Vector3d & a_HitPos);
	
} ; // tolua_export
