
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Creeper.h"
#include "../World.h"
#include "../Entities/ProjectileEntity.h"
#include "../Entities/Player.h"





cCreeper::cCreeper(void) :
	Super("Creeper", mtCreeper, "entity.creeper.hurt", "entity.creeper.death", "entity.creeper.ambient", 0.6, 1.8),
	m_bIsBlowing(false),
	m_bIsCharged(false),
	m_BurnedWithFlintAndSteel(false),
	m_ExplodingTimer(0)
{
}





void cCreeper::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (((GetTarget() == nullptr) || !TargetIsInRange()) && !m_BurnedWithFlintAndSteel)
	{
		if (m_bIsBlowing)
		{
			m_ExplodingTimer = 0;
			m_bIsBlowing = false;
			m_World->BroadcastEntityMetadata(*this);
		}
	}
	else
	{
		if (m_bIsBlowing)
		{
			m_ExplodingTimer += 1;
		}

		if ((m_ExplodingTimer == 30) && (GetHealth() > 0.0))  // only explode when not already dead
		{
			m_World->DoExplosionAt((m_bIsCharged ? 5 : 3), GetPosX(), GetPosY(), GetPosZ(), false, esMonster, this);
			Destroy();  // Just in case we aren't killed by the explosion
		}
	}
}





void cCreeper::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (m_ExplodingTimer == 30)
	{
		// Exploded creepers drop naught but charred flesh, which Minecraft doesn't have
		return;
	}

	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER);

	// If the creeper was killed by a skeleton, add a random music disc drop:
	if (
		(a_Killer != nullptr) &&
		a_Killer->IsProjectile() &&
		((static_cast<cProjectileEntity *>(a_Killer))->GetCreatorUniqueID() != cEntity::INVALID_ID))
	{
		auto ProjectileCreatorCallback = [](cEntity & a_Entity)
			{
				return (
					a_Entity.IsMob() &&
					((static_cast<cMonster &>(a_Entity)).GetMobType() == mtSkeleton)
				);
			};

		if (GetWorld()->DoWithEntityByID(static_cast<cProjectileEntity *>(a_Killer)->GetCreatorUniqueID(), ProjectileCreatorCallback))
		{
			AddRandomDropItem(a_Drops, 1, 1, static_cast<short>(m_World->GetTickRandomNumber(11) + E_ITEM_FIRST_DISC));
		}
	}
}





bool cCreeper::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if (a_TDI.DamageType == dtLightning)
	{
		m_bIsCharged = true;
	}

	m_World->BroadcastEntityMetadata(*this);
	return true;
}





bool cCreeper::Attack(std::chrono::milliseconds a_Dt)
{
	UNUSED(a_Dt);

	if (!m_bIsBlowing)
	{
		m_World->BroadcastSoundEffect("entity.creeper.primed", GetPosition(), 1.f, (0.75f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
		m_bIsBlowing = true;
		m_World->BroadcastEntityMetadata(*this);

		return true;
	}
	return false;
}





void cCreeper::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	if ((a_Player.GetEquippedItem().m_ItemType == E_ITEM_FLINT_AND_STEEL))
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.UseEquippedItem();
		}
		m_World->BroadcastSoundEffect("entity.creeper.primed", GetPosition(), 1.f, (0.75f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
		m_bIsBlowing = true;
		m_World->BroadcastEntityMetadata(*this);
		m_BurnedWithFlintAndSteel = true;
	}
}
