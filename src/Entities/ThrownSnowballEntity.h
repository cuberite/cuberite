
// ThrownSnowballEntity.h

// Declares the cThrownSnowballEntity representing a snowball that has been thrown





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownSnowballEntity :
	public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cThrownSnowballEntity)

	cThrownSnowballEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

protected:

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

private:

	/** Time in ticks to wait for the hit animation to begin before destroying */
	int m_DestroyTimer;

} ;  // tolua_export




