#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownSnowballEntity.h"
#include "../World.h"





cThrownSnowballEntity::cThrownSnowballEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkSnowball, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
}





void cThrownSnowballEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	m_DestroyTimer = 2;
}





void cThrownSnowballEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	super::OnHitEntity(a_EntityHit, a_HitPos);

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
		super::Tick(a_Dt, a_Chunk);
	}
}




