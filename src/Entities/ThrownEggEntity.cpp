#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownEggEntity.h"
#include "../World.h"





cThrownEggEntity::cThrownEggEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEgg, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
}





void cThrownEggEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	TrySpawnChicken(a_HitPos);
	
	m_DestroyTimer = 2;
}





void cThrownEggEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	int TotalDamage = 0;
	// TODO: If entity is Ender Crystal, destroy it
	
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





void cThrownEggEntity::TrySpawnChicken(const Vector3d & a_HitPos)
{
	if (m_World->GetTickRandomNumber(7) == 1)
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, false);
	}
	else if (m_World->GetTickRandomNumber(32) == 1)
	{
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, false);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, false);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, false);
		m_World->SpawnMob(a_HitPos.x, a_HitPos.y, a_HitPos.z, mtChicken, false);
	}
}
