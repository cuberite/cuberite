#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Player.h"
#include "ArrowEntity.h"
#include "../Chunk.h"





cArrowEntity::cArrowEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed) :
	super(pkArrow, a_Creator, a_X, a_Y, a_Z, 0.5, 0.5),
	m_PickupState(psNoPickup),
	m_DamageCoeff(2),
	m_IsCritical(false),
	m_Timer(0),
	m_HitGroundTimer(0),
	m_HasTeleported(false),
	m_bIsCollected(false),
	m_HitBlockPos(Vector3i(0, 0, 0))
{
	SetSpeed(a_Speed);
	SetMass(0.1);
	SetGravity(-20.0f);
	SetAirDrag(0.2f);
	SetYawFromSpeed();
	SetPitchFromSpeed();
	LOGD("Created arrow %d with speed {%.02f, %.02f, %.02f} and rot {%.02f, %.02f}",
		m_UniqueID, GetSpeedX(), GetSpeedY(), GetSpeedZ(),
		GetYaw(), GetPitch()
	);
}





cArrowEntity::cArrowEntity(cPlayer & a_Player, double a_Force) :
	super(pkArrow, &a_Player, a_Player.GetThrowStartPos(), a_Player.GetThrowSpeed(a_Force * 1.5 * 20), 0.5, 0.5),
	m_PickupState(psInSurvivalOrCreative),
	m_DamageCoeff(2),
	m_IsCritical((a_Force >= 1)),
	m_Timer(0),
	m_HitGroundTimer(0),
	m_HasTeleported(false),
	m_bIsCollected(false),
	m_HitBlockPos(0, 0, 0)
{
	if (a_Player.IsGameModeCreative())
	{
		m_PickupState = psInCreative;
	}
	SetGravity(-20.0f);
	SetAirDrag(0.01f);
}





bool cArrowEntity::CanPickup(const cPlayer & a_Player) const
{
	switch (m_PickupState)
	{
		case psNoPickup:             return false;
		case psInSurvivalOrCreative: return (a_Player.IsGameModeSurvival() || a_Player.IsGameModeCreative());
		case psInCreative:           return a_Player.IsGameModeCreative();
	}
	UNREACHABLE("Unsupported arrow pickup state");
}





void cArrowEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Vector3d Hit = a_HitPos;
	Hit += GetSpeed().NormalizeCopy() / 100000;  // Make arrow sink into block a bit so it lodges (TODO: investigate how to stop them going so far so that they become black clientside)

	super::OnHitSolidBlock(Hit, a_HitFace);
	Vector3i BlockHit = Hit.Floor();

	int X = BlockHit.x, Y = BlockHit.y, Z = BlockHit.z;
	m_HitBlockPos = Vector3i(X, Y, Z);

	// Broadcast arrow hit sound
	m_World->BroadcastSoundEffect("entity.arrow.hit", BlockHit, 0.5f, static_cast<float>(0.75 + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));

	if ((m_World->GetBlock(Hit) == E_BLOCK_TNT) && IsOnFire())
	{
		m_World->SetBlock(X, Y, Z, E_BLOCK_AIR, 0);
		m_World->SpawnPrimedTNT(Vector3d(BlockHit));
	}

}





void cArrowEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	super::OnHitEntity(a_EntityHit, a_HitPos);

	int Damage = static_cast<int>(GetSpeed().Length() / 20 * m_DamageCoeff + 0.5);
	if (m_IsCritical)
	{
		Damage += m_World->GetTickRandomNumber(Damage / 2 + 2);
	}

	unsigned int PowerLevel = m_CreatorData.m_Enchantments.GetLevel(cEnchantments::enchPower);
	if (PowerLevel > 0)
	{
		int ExtraDamage = static_cast<int>(ceil(0.25 * (PowerLevel + 1)));
		Damage += ExtraDamage;
	}

	unsigned int PunchLevel = m_CreatorData.m_Enchantments.GetLevel(cEnchantments::enchPunch);
	double KnockbackAmount = 11 + 10 * PunchLevel;
	a_EntityHit.TakeDamage(dtRangedAttack, GetCreatorUniqueID(), Damage, KnockbackAmount);

	if (IsOnFire() && !a_EntityHit.IsInWater())
	{
		a_EntityHit.StartBurning(100);
	}

	// Broadcast successful hit sound
	GetWorld()->BroadcastSoundEffect("entity.arrow.hit_player", GetPosition(), 0.5, static_cast<float>(0.75 + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));

	Destroy();
}





void cArrowEntity::CollectedBy(cPlayer & a_Dest)
{
	if (m_IsInGround && !m_bIsCollected && CanPickup(a_Dest))
	{
		// Do not add the arrow to the inventory when the player is in creative:
		if (!a_Dest.IsGameModeCreative())
		{
			int NumAdded = a_Dest.GetInventory().AddItem(E_ITEM_ARROW);
			if (NumAdded == 0)
			{
				// No space in the inventory
				return;
			}
		}

		GetWorld()->BroadcastCollectEntity(*this, a_Dest, 1);
		GetWorld()->BroadcastSoundEffect("entity.item.pickup", GetPosition(), 0.5, static_cast<float>(0.75 + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
		m_bIsCollected = true;
	}
}





void cArrowEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	m_Timer += a_Dt;

	if (m_bIsCollected)
	{
		if (m_Timer > std::chrono::milliseconds(500))
		{
			Destroy();
			return;
		}
	}
	else if (m_Timer > std::chrono::minutes(5))
	{
		Destroy();
		return;
	}

	if (m_IsInGround)
	{
		if (!m_HasTeleported)  // Sent a teleport already, don't do again
		{
			if (m_HitGroundTimer > std::chrono::milliseconds(500))
			{
				m_World->BroadcastTeleportEntity(*this);
				m_HasTeleported = true;
			}
			else
			{
				m_HitGroundTimer += a_Dt;
			}
		}

		int RelPosX = m_HitBlockPos.x - a_Chunk.GetPosX() * cChunkDef::Width;
		int RelPosZ = m_HitBlockPos.z - a_Chunk.GetPosZ() * cChunkDef::Width;
		cChunk * Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(RelPosX, RelPosZ);

		if (Chunk == nullptr)
		{
			// Inside an unloaded chunk, abort
			return;
		}

		if (Chunk->GetBlock(RelPosX, m_HitBlockPos.y, RelPosZ) == E_BLOCK_AIR)  // Block attached to was destroyed?
		{
			m_IsInGround = false;  // Yes, begin simulating physics again
		}
	}
}
