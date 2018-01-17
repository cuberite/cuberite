#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "GhastFireballEntity.h"
#include "../World.h"





cGhastFireballEntity::cGhastFireballEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkGhastFireball, a_Creator, a_X, a_Y, a_Z, 1, 1)
{
	SetSpeed(a_Speed);
	SetGravity(0);
	SetAirDrag(0);
}





void cGhastFireballEntity::Explode(Vector3i a_Block)
{
	m_World->DoExplosionAt(1, a_Block.x, a_Block.y, a_Block.z, true, esGhastFireball, this);
}





void cGhastFireballEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Destroy();
	Explode(a_HitPos.Floor());
}





void cGhastFireballEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Destroy();
	Explode(a_HitPos.Floor());
}
