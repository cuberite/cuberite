#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FireChargeEntity.h"
#include "../World.h"
#include "BlockInfo.h"




cFireChargeEntity::cFireChargeEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(pkFireCharge, a_Creator, a_Pos, 0.3125f, 0.3125f)
{
	SetSpeed(a_Speed);
	SetGravity(0);
	SetAirDrag(0);
}





void cFireChargeEntity::Explode(Vector3i a_Block)
{
	if (IsBlockAir(m_World->GetBlock(a_Block)))
	{
		m_World->SetBlock(a_Block, Block::Fire::Fire());
	}
}





void cFireChargeEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Destroy();
	Explode(a_HitPos.Floor());
}





void cFireChargeEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Super::OnHitEntity(a_EntityHit, a_HitPos);

	Destroy();
	Explode(a_HitPos.Floor());

	if (!a_EntityHit.IsFireproof())
	{
		// TODO Damage Entity with 5 damage(from https://minecraft.wiki/w/Blaze#Blaze_fireball)
		a_EntityHit.StartBurning(5 * 20);  // 5 seconds of burning
	}
}
