#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Player.h"
#include "ArrowEntity.h"
#include "../Chunk.h"
#include "../Blocks/BlockButton.h"





cArrowEntity::cArrowEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(pkArrow, a_Creator, a_Pos, 0.5, 0.5),
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
	SetAirDrag(0.01f);
	SetYawFromSpeed();
	SetPitchFromSpeed();
	FLOGD("Created arrow {0} with speed {1:.02f} and rot {{{2:.02f}, {3:.02f}}}",
		m_UniqueID, GetSpeed(), GetYaw(), GetPitch()
	);
}





cArrowEntity::cArrowEntity(cPlayer & a_Player, double a_Force) :
	Super(pkArrow, &a_Player, a_Player.GetThrowStartPos(), a_Player.GetThrowSpeed(a_Force * 1.5 * 20), 0.5, 0.5),
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
	// Save the direction we're going in before Super resets it
	auto Speed = GetSpeed();
	Speed.Normalize();

	Super::OnHitSolidBlock(a_HitPos, a_HitFace);

	/*
	The line tracer returns the arrow hit position located on the face of a block;
	if the arrow hit a block at -5, 95, -5 then a_HitPos would start off as -4, 95.5, -4.1,
	i.e. it collided with the X face.

	Then we add a bit of speed vector to make a_HitPos -4.0001, 95.5, -4.1
	and floor to get exactly -5, 95, -5 which is stored in m_HitBlockPos.
	*/

	// Nudge into the block a tiny bit according to its direction of travel
	// Floor to give the coordinates of the block it crashed into
	a_HitPos += Speed / 100000;
	m_HitBlockPos = a_HitPos.Floor();

	// Broadcast arrow hit sound
	m_World->BroadcastSoundEffect("entity.arrow.hit", m_HitBlockPos, 0.5f, static_cast<float>(0.75 + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));

	// To make buttons get pressed if they got hit by the arrow
	m_World->WakeUpSimulators(m_HitBlockPos);

	if ((m_World->GetBlock(m_HitBlockPos) == E_BLOCK_TNT) && IsOnFire())
	{
		m_World->SetBlock(m_HitBlockPos, E_BLOCK_AIR, 0);
		m_World->SpawnPrimedTNT(m_HitBlockPos);
	}

	cBlockButtonHandler::OnArrowHit(*m_World, m_HitBlockPos, a_HitFace);
}





void cArrowEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Super::OnHitEntity(a_EntityHit, a_HitPos);

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

	double Knockback = 10;

	unsigned int PunchLevel = m_CreatorData.m_Enchantments.GetLevel(cEnchantments::enchPunch);
	unsigned int PunchLevelMultiplier = 8;

	Knockback += PunchLevelMultiplier * PunchLevel;
	a_EntityHit.TakeDamage(dtRangedAttack, GetCreatorUniqueID(), Damage, Knockback);

	if (IsOnFire() && !a_EntityHit.IsInWater())
	{
		a_EntityHit.StartBurning(100);
	}

	// Broadcast successful hit sound
	GetWorld()->BroadcastSoundEffect("entity.arrow.hit", GetPosition(), 0.5, static_cast<float>(0.75 + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));

	Destroy();
}





void cArrowEntity::CollectedBy(cPlayer & a_Dest)
{
	if (m_IsInGround && !m_bIsCollected && CanPickup(a_Dest))
	{
		// Do not add the arrow to the inventory when the player is in creative:
		if (!a_Dest.IsGameModeCreative())
		{
			int NumAdded = a_Dest.GetInventory().AddItem(cItem(E_ITEM_ARROW));
			if (NumAdded == 0)
			{
				// No space in the inventory
				return;
			}
		}

		GetWorld()->BroadcastCollectEntity(*this, a_Dest, 1);
		GetWorld()->BroadcastSoundEffect("entity.item.pickup", GetPosition(), 0.3f, (1.2f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
		m_bIsCollected = true;
	}
}





void cArrowEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
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

		auto RelPos = a_Chunk.AbsoluteToRelative(m_HitBlockPos);
		auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(RelPos);

		if (Chunk == nullptr)
		{
			// Inside an unloaded chunk, abort
			return;
		}

		if (Chunk->GetBlock(RelPos) == E_BLOCK_AIR)  // Block attached to was destroyed?
		{
			m_IsInGround = false;  // Yes, begin simulating physics again
		}
	}
}
