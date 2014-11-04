//
//  GhastFireballEntity.h
//

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
	
	cGhastFireballEntity(CreateEntityInfo a_Info, cEntity * a_Creator);
	
protected:
	
	void Explode(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	
	// TODO: Deflecting the fireballs by arrow- or sword- hits
	
} ;  // tolua_export
