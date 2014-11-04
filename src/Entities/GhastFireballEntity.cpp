#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "GhastFireballEntity.h"
#include "../World.h"





cGhastFireballEntity::cGhastFireballEntity(CreateEntityInfo a_Info, cEntity * a_Creator) :
	super(a_Info, pkGhastFireball, a_Creator, 1, 1)
{
	SetGravity(0);
}





void cGhastFireballEntity::Explode(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	m_World->DoExplosionAt(1, a_BlockX, a_BlockY, a_BlockZ, true, esGhastFireball, this);
}





void cGhastFireballEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
}





void cGhastFireballEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
}
