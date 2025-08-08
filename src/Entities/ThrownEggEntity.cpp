#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownEggEntity.h"
#include "../World.h"





cThrownEggEntity::cThrownEggEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(etEgg, a_Creator, a_Pos, a_Speed, 0.25f, 0.25f)
{
}





void cThrownEggEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Super::OnHitEntity(a_EntityHit, a_HitPos);

	int Damage = 0;
	if (a_EntityHit.IsMob() && (static_cast<cMonster &>(a_EntityHit).GetEntityType() == etEnderDragon))
	{
		// Enderdragons take 1 damage:
		Damage = 1;
	}
	else if (a_EntityHit.IsEnderCrystal())
	{
		// Endercrystals are destroyed:
		Damage = CeilC(a_EntityHit.GetHealth());
	}

	a_EntityHit.TakeDamage(dtRangedAttack, GetCreatorUniqueID(), Damage, 1);
	m_World->BroadcastEntityAnimation(*this, EntityAnimation::EggCracks);

	TrySpawnChicken(a_HitPos);
	Destroy();
}





void cThrownEggEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Super::OnHitSolidBlock(a_HitPos, a_HitFace);

	m_World->BroadcastEntityAnimation(*this, EntityAnimation::EggCracks);

	TrySpawnChicken(a_HitPos);
	Destroy();
}





void cThrownEggEntity::TrySpawnChicken(Vector3d a_HitPos)
{
	auto & Random = GetRandomProvider();
	if (Random.RandBool(0.125))
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, etChicken, true);
	}
	else if (Random.RandBool(1.0 / 33.0))
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, etChicken, true);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, etChicken, true);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, etChicken, true);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, etChicken, true);
	}
}
