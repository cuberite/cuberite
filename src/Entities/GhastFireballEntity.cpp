#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "GhastFireballEntity.h"
#include "../World.h"





cGhastFireballEntity::cGhastFireballEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(pkGhastFireball, a_Creator, a_Pos, 1, 1)
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
