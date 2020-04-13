#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ExpBottleEntity.h"
#include "../World.h"
#include "../EffectID.h"





cExpBottleEntity::cExpBottleEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed) :
	Super(pkExpBottle, a_Creator, a_Pos, 0.25, 0.25)
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
	m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SPLASH_POTION, GetPosition().Floor(), 0);
	m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), GetRandomProvider().RandInt(3, 11));
	Destroy();
}
