#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ExpBottleEntity.h"
#include "../World.h"
#include "../EffectID.h"





cExpBottleEntity::cExpBottleEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, Vector3d a_Speed) :
	super(pkExpBottle, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





cExpBottleEntity::cExpBottleEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed) :
	super(pkExpBottle, a_Creator, a_Pos.x, a_Pos.y, a_Pos.z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cExpBottleEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Break(a_HitPos);
}





void cExpBottleEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Break(a_HitPos);
}





void cExpBottleEntity::Break(Vector3d a_HitPos)
{
	// Spawn an experience orb with a reward between 3 and 11.
	m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SPLASH_POTION, POSX_TOINT, POSY_TOINT, POSZ_TOINT, 0);
	m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), GetRandomProvider().RandInt(3, 11));
	Destroy();
}
