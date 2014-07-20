//
//  FireChargeEntity.h
//

#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cFireChargeEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	
	// tolua_end
	
	CLASS_PROTODEF(cFireChargeEntity);
	
	cFireChargeEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);
	
protected:
	
	void Explode(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	
} ;  // tolua_export
