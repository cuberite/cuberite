#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SplashPotionEntity.h"
#include "../World.h"





cSplashPotionEntity::cSplashPotionEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
super(pkSplashPotion, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cSplashPotionEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// TODO: Apply potion effect to entities nearby
	Destroy();
}





void cSplashPotionEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	a_EntityHit.TakeDamage(dtRangedAttack, this, 0, 1);
	
	// TODO: Apply potion effect to entity and others nearby
	
	Destroy(true);
}
