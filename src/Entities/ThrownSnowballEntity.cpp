#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownSnowballEntity.h"
#include "../World.h"





cThrownSnowballEntity::cThrownSnowballEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkSnowball, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
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
		cMonster::eType MobType = ((cMonster &) a_EntityHit).GetMobType();
		if (MobType == cMonster::mtBlaze)
		{
			TotalDamage = 3;
		}
	}
	// TODO: If entity is Ender Crystal, destroy it
	a_EntityHit.TakeDamage(dtRangedAttack, this, TotalDamage, 1);
	
	m_DestroyTimer = 5;
}
