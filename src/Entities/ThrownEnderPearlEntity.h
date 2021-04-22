
// ThrownEnderPearlEntity.h

// Declares the cThrownEnderPeralEntity class representing an ender pearl being thrown





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownEnderPearlEntity :
	public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cThrownEnderPearlEntity)

	cThrownEnderPearlEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

private:

	/** Teleports the creator where the ender pearl lands */
	void TeleportCreator(Vector3d a_HitPos);

	// cProjectileEntity overrides:
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;

} ;  // tolua_export
