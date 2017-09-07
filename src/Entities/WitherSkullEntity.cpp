
//  WitherSkullEntity.cpp

// Implements the cWitherSkullEntity class representing the entity used by both blue and black wither skulls

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WitherSkullEntity.h"
#include "../World.h"





cWitherSkullEntity::cWitherSkullEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkWitherSkull, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
	SetGravity(0);
	SetAirDrag(0);
}





void cWitherSkullEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	// TODO: Explode
	// TODO: Apply wither effect to entities nearby
	Destroy();
}





void cWitherSkullEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	// TODO: If entity is Ender Crystal, destroy it
	a_EntityHit.TakeDamage(dtRangedAttack, this, 0, 1);

	// TODO: Explode
	// TODO: Apply wither effect to entity and others nearby

	Destroy(true);
}




