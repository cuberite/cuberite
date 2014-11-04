#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownSnowballEntity.h"
#include "../World.h"





cThrownSnowballEntity::cThrownSnowballEntity(CreateEntityInfo a_Info, cEntity * a_Creator) :
	super(a_Info, pkSnowball, a_Creator, 0.25, 0.25),
	m_DestroyTimer(-1)
{
}





void cThrownSnowballEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	m_DestroyTimer = 2;
}





void cThrownSnowballEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	int TotalDamage = 0;
	if (a_EntityHit.IsMob())
	{
		eMonsterType MobType = ((cMonster &) a_EntityHit).GetMobType();
		if (MobType == mtBlaze)
		{
			TotalDamage = 3;
		}
	}
	// TODO: If entity is Ender Crystal, destroy it
	a_EntityHit.TakeDamage(dtRangedAttack, this, TotalDamage, 1);
	
	m_DestroyTimer = 5;
}
