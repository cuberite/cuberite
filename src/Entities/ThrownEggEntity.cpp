#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownEggEntity.h"
#include "../World.h"





cThrownEggEntity::cThrownEggEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEgg, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
}





void cThrownEggEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	TrySpawnChicken(a_HitPos);

	m_DestroyTimer = 2;
}





void cThrownEggEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	int TotalDamage = 0;
	// If entity is an Ender Dragon or Ender Crystal, it is damaged.
	if (
		(a_EntityHit.IsMob() && (static_cast<cMonster &>(a_EntityHit).GetMobType() == mtEnderDragon)) ||
		a_EntityHit.IsEnderCrystal()
	)
	{
		TotalDamage = 1;
	}

	TrySpawnChicken(a_HitPos);
	a_EntityHit.TakeDamage(dtRangedAttack, this, TotalDamage, 1);

	m_DestroyTimer = 5;
}





void cThrownEggEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_DestroyTimer > 0)
	{
		m_DestroyTimer--;
		if (m_DestroyTimer == 0)
		{
			Destroy();
			return;
		}
	}
	else
	{
		super::Tick(a_Dt, a_Chunk);
	}
}





void cThrownEggEntity::TrySpawnChicken(Vector3d a_HitPos)
{
	auto & Random = GetRandomProvider();
	if (Random.RandBool(0.125))
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, true);
	}
	else if (Random.RandBool(1.0 / 33.0))
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, true);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, true);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, true);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, true);
	}
}




