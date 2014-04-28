#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ExpBottleEntity.h"
#include "../World.h"





cExpBottleEntity::cExpBottleEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkExpBottle, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cExpBottleEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// Spawn an experience orb with a reward between 3 and 11.
	m_World->BroadcastSoundParticleEffect(2002, POSX_TOINT, POSY_TOINT, POSZ_TOINT, 0);
	m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), 3 + m_World->GetTickRandomNumber(8));
	
	Destroy();
}
