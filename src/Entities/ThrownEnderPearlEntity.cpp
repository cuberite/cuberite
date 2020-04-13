
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownEnderPearlEntity.h"
#include "Player.h"





cThrownEnderPearlEntity::cThrownEnderPearlEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(pkEnderPearl, a_Creator, a_Pos, 0.25, 0.25),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
}





void cThrownEnderPearlEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	// TODO: Tweak a_HitPos based on block face.
	TeleportCreator(a_HitPos);

	m_DestroyTimer = 2;
}





void cThrownEnderPearlEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	int TotalDamage = 0;
	// TODO: If entity is Ender Crystal, destroy it

	TeleportCreator(a_HitPos);
	a_EntityHit.TakeDamage(dtRangedAttack, this, TotalDamage, 1);

	m_DestroyTimer = 5;
}





void cThrownEnderPearlEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
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





void cThrownEnderPearlEntity::TeleportCreator(Vector3d a_HitPos)
{
	if (m_CreatorData.m_Name.empty())
	{
		return;
	}

	GetWorld()->FindAndDoWithPlayer(m_CreatorData.m_Name, [=](cPlayer & a_Entity)
		{
			// Teleport the creator here, make them take 5 damage:
			a_Entity.TeleportToCoords(a_HitPos.x, a_HitPos.y + 0.2, a_HitPos.z);
			a_Entity.TakeDamage(dtEnderPearl, this, 5, 0);
			return true;
		}
	);
}
