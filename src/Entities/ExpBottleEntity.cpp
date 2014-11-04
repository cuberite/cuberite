#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ExpBottleEntity.h"
#include "../World.h"





cExpBottleEntity::cExpBottleEntity(CreateEntityInfo a_Info, cEntity * a_Creator) :
	super(a_Info, pkExpBottle, a_Creator, 0.25, 0.25)
{
}





void cExpBottleEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	Break(a_HitPos);
}





void cExpBottleEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	Break(a_HitPos);
}





void cExpBottleEntity::Break(const Vector3d &a_HitPos)
{
	// Spawn an experience orb with a reward between 3 and 11.
	m_World->BroadcastSoundParticleEffect(2002, POSX_TOINT, POSY_TOINT, POSZ_TOINT, 0);
	m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), 3 + m_World->GetTickRandomNumber(8));
	Destroy();
}
