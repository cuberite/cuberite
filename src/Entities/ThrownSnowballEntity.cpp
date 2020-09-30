#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownSnowballEntity.h"
#include "../World.h"





cThrownSnowballEntity::cThrownSnowballEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(pkSnowball, a_Creator, a_Pos, 0.25, 0.25),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
}





void cThrownSnowballEntity::CopyFrom(const cEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	const auto & SnowBall = static_cast<const cThrownSnowballEntity &>(a_Src);
	m_DestroyTimer = SnowBall.m_DestroyTimer;
}





void cThrownSnowballEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	m_DestroyTimer = 2;
}





void cThrownSnowballEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Super::OnHitEntity(a_EntityHit, a_HitPos);

	int TotalDamage = 0;
	if (a_EntityHit.IsMob())
	{
		eMonsterType MobType = static_cast<cMonster &>(a_EntityHit).GetMobType();
		if (MobType == mtBlaze)
		{
			TotalDamage = 3;
		}
	}
	// TODO: If entity is Ender Crystal, destroy it
	a_EntityHit.TakeDamage(dtRangedAttack, GetCreatorUniqueID(), TotalDamage, 1);

	m_DestroyTimer = 5;
}





void cThrownSnowballEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
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
		Super::Tick(a_Dt, a_Chunk);
	}
}




