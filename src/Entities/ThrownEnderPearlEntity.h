//
//  ThrownEnderPearlEntity.h
//

#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownEnderPearlEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	
	// tolua_end
	
	CLASS_PROTODEF(cThrownEnderPearlEntity);
	
	cThrownEnderPearlEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);
	
protected:
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	
	// Teleports the creator where the ender pearl lands.
	void TeleportCreator(const Vector3d & a_HitPos);
	
} ; // tolua_export
