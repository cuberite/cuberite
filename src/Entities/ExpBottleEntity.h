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
	
	CLASS_PROTODEF(cExpBottleEntity)
	
	cExpBottleEntity(CreateEntityInfo a_Info, cEntity * a_Creator);
	
protected:
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;

	/** Breaks the bottle, fires its particle effects and sounds
	@param a_HitPos     The position where the bottle will break */
	void Break(const Vector3d &a_HitPos);
	
};  // tolua_export
