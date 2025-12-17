
// GhastFireballEntity.h

// Declares the cGhastFireballEntity class representing the ghast fireball in flight.





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cGhastFireballEntity :
	public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cGhastFireballEntity)

	cGhastFireballEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

	cEntity * GetShooter(void) { return m_Shooter; };

protected:

	void Explode(Vector3i a_Block);

	void ReflectFromAttack(cEntity * a_AttackingEnt);
	Vector3d NotchReflect(cEntity & a_AttackingEnt);

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;

	// cEntity overrides:
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

private:

	cEntity * m_Shooter;

} ;  // tolua_export




