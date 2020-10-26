#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Player.h"
#include "ArrowEntity.h"
#include "../Chunk.h"
#include "../Blocks/BlockButton.h"





cArrowEntity::cArrowEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed):
	Super(pkArrow, a_Creator, a_Pos, a_Speed, 0.5, 0.5),
	m_PickupState(psNoPickup),
	m_DamageCoeff(2),
	m_IsCritical(false),
	m_Timer(0),
	m_bIsCollected(false)
{
	SetMass(0.1);
	SetGravity(-20.0f);

	FLOGD("Created arrow {0} with speed {1:.02f} and rot {{{2:.02f}, {3:.02f}}}",
		m_UniqueID, GetSpeed(), GetYaw(), GetPitch()
	);
}





cArrowEntity::cArrowEntity(cPlayer & a_Player, double a_Force) :
	cArrowEntity(&a_Player, a_Player.GetThrowStartPos(), a_Player.GetThrowSpeed(a_Force * 1.5 * 20))
{
	m_IsCritical = a_Force >= 1;
	m_PickupState = a_Player.IsGameModeCreative() ? psInCreative : psInSurvivalOrCreative;
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

	/*
	The line tracer returns the arrow hit position located on the face of a block;
	if the arrow hit a block at -5, 95, -5 then a_HitPos would start off as -4, 95.5, -4.1,
	i.e. it collided with the X face.

	First we subtract a bit of speed vector so it doesn't appear black clientside.

	Then we add a bit of speed vector to make a_HitPos -4.0001, 95.5, -4.1
	and floor to get exactly -5, 95, -5 which is stored in m_HitBlockPos.
	*/

	// Shift the arrow's position slightly back so that less than 50% of the hitbox
	// is in the block. Otherwise the arrow can appear black
	Super::OnHitSolidBlock(a_HitPos - (Speed / 100), a_HitFace);

	// Nudge into the block a tiny bit according to its direction of travel
	// Floor to give the coordinates of the block it crashed into
	m_HitBlockPos = (a_HitPos + (Speed / 100000)).Floor();

	// Broadcast arrow hit sound
	m_World->BroadcastSoundEffect("entity.arrow.hit", m_HitBlockPos, 0.5f, static_cast<float>(0.75 + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));

	// Trigger any buttons that were hit
	// Wooden buttons will be depressed by the arrow
	cBlockButtonHandler::OnArrowHit(*m_World, m_HitBlockPos, a_HitFace);

	if ((m_World->GetBlock(m_HitBlockPos) == E_BLOCK_TNT) && IsOnFire())
	{
		m_World->SetBlock(m_HitBlockPos, E_BLOCK_AIR, 0);
		m_World->SpawnPrimedTNT(m_HitBlockPos);
	}
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
		if (m_World->GetBlock(m_HitBlockPos) == E_BLOCK_AIR)  // Block attached to was destroyed?
		{
			m_IsInGround = false;  // Yes, begin simulating physics again
		}
	}
}





bool cArrowEntity::DoesPreventBlockPlacement(void) const
{
	return false;
}
