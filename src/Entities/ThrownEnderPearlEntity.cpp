#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownEnderPearlEntity.h"





cThrownEnderPearlEntity::cThrownEnderPearlEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEnderPearl, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25)
{
	SetSpeed(a_Speed);
}





void cThrownEnderPearlEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// TODO: Tweak a_HitPos based on block face.
	TeleportCreator(a_HitPos);
	
	Destroy();
}





void cThrownEnderPearlEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	int TotalDamage = 0;
	// TODO: If entity is Ender Crystal, destroy it
	
	TeleportCreator(a_HitPos);
	a_EntityHit.TakeDamage(dtRangedAttack, this, TotalDamage, 1);
	
	Destroy(true);
}





void cThrownEnderPearlEntity::TeleportCreator(const Vector3d & a_HitPos)
{
	// Teleport the creator here, make them take 5 damage:
	if (m_Creator != NULL)
	{
		m_Creator->TeleportToCoords(a_HitPos.x + 0.5, a_HitPos.y + 1.7, a_HitPos.z + 0.5);
		m_Creator->TakeDamage(dtEnderPearl, this, 5, 0);
	}
}
