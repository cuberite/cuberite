#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownSnowballEntity.h"
#include "../World.h"





cThrownSnowballEntity::cThrownSnowballEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(etSnowball, a_Creator, a_Pos, a_Speed, 0.25f, 0.25f)
{
}





void cThrownSnowballEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Super::OnHitEntity(a_EntityHit, a_HitPos);

	int Damage = 0;
	if (a_EntityHit.IsMob() && (static_cast<cMonster &>(a_EntityHit).GetEntityType() == etBlaze))
	{
		// Blazes take 3 damage:
		Damage = 3;
	}
	else if (a_EntityHit.IsEnderCrystal())
	{
		// Endercrystals are destroyed:
		Damage = CeilC(a_EntityHit.GetHealth());
	}

	a_EntityHit.TakeDamage(dtRangedAttack, GetCreatorUniqueID(), Damage, 1);
	m_World->BroadcastEntityAnimation(*this, EntityAnimation::SnowballPoofs);
	Destroy();
}





void cThrownSnowballEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Super::OnHitSolidBlock(a_HitPos, a_HitFace);

	m_World->BroadcastEntityAnimation(*this, EntityAnimation::SnowballPoofs);
	Destroy();
}
