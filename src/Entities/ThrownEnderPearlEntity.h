
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

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	/** Teleports the creator where the ender pearl lands */
	void TeleportCreator(Vector3d a_HitPos);

private:

	/** Time in ticks to wait for the hit animation to begin before destroying */
	int m_DestroyTimer;

} ;  // tolua_export




