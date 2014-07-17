//
//  ExpBottleEntity.h
//

#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cExpBottleEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	
	// tolua_end
	
	CLASS_PROTODEF(cExpBottleEntity);
	
	cExpBottleEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);
	
protected:
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	
};  // tolua_export
