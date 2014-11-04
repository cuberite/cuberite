
//  WitherSkullEntity.h

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
	
	cWitherSkullEntity(CreateEntityInfo a_Info, cEntity * a_Creator);
	
protected:
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	
} ;  // tolua_export
