#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ThrownEnderPearlEntity.h"
#include "Player.h"





cThrownEnderPearlEntity::cThrownEnderPearlEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkEnderPearl, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
}





void cThrownEnderPearlEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	// TODO: Tweak a_HitPos based on block face.
	TeleportCreator(a_HitPos);
	
	m_DestroyTimer = 2;
}





void cThrownEnderPearlEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	int TotalDamage = 0;
	// TODO: If entity is Ender Crystal, destroy it
	
	TeleportCreator(a_HitPos);
	a_EntityHit.TakeDamage(dtRangedAttack, this, TotalDamage, 1);
	
	m_DestroyTimer = 5;
}





void cThrownEnderPearlEntity::TeleportCreator(const Vector3d & a_HitPos)
{
	if (m_CreatorData.m_Name.empty())
	{
		return;
	}

	class cProjectileCreatorCallbackForPlayers : public cPlayerListCallback
	{
	public:
		cProjectileCreatorCallbackForPlayers(cEntity * a_Attacker, Vector3i a_HitPos) :
			m_Attacker(a_Attacker),
			m_HitPos(a_HitPos)
		{
		}

		virtual bool Item(cPlayer * a_Entity) override
		{
			// Teleport the creator here, make them take 5 damage:
			a_Entity->TeleportToCoords(m_HitPos.x, m_HitPos.y + 0.2, m_HitPos.z);
			a_Entity->TakeDamage(dtEnderPearl, m_Attacker, 5, 0);
			return true;
		}

	private:

		cEntity * m_Attacker;
		Vector3i m_HitPos;
	};

	cProjectileCreatorCallbackForPlayers PCCFP(this, a_HitPos);
	GetWorld()->FindAndDoWithPlayer(m_CreatorData.m_Name, PCCFP);
}
