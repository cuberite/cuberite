//
//  ThrownSnowballEntity.h
//

#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cThrownSnowballEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	
	// tolua_end
	
	CLASS_PROTODEF(cThrownSnowballEntity);
	
	cThrownSnowballEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed);
	
protected:
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	virtual void Tick           (float a_Dt, cChunk & a_Chunk) override
	{
		if (m_DestroyTimer > 0)
		{
			m_DestroyTimer--;
			if (m_DestroyTimer == 0)
			{
				Destroy();
				return;
			}
		}
		else
		{
			super::Tick(a_Dt, a_Chunk);
		}
	}

private:

	/** Time in ticks to wait for the hit animation to begin before destroying */
	int m_DestroyTimer;
	
} ;  // tolua_export
