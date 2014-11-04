#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FireChargeEntity.h"
#include "../World.h"





cFireChargeEntity::cFireChargeEntity(CreateEntityInfo a_Info, cEntity * a_Creator) :
	super(a_Info, pkFireCharge, a_Creator, 0.3125, 0.3125)
{
	SetGravity(0);
}





void cFireChargeEntity::Explode(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (m_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_AIR)
	{
		m_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FIRE, 1);
	}
}





void cFireChargeEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
}





void cFireChargeEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	Destroy();
	Explode((int)floor(a_HitPos.x), (int)floor(a_HitPos.y), (int)floor(a_HitPos.z));
	
	// TODO: Some entities are immune to hits
	a_EntityHit.StartBurning(5 * 20);  // 5 seconds of burning
}
