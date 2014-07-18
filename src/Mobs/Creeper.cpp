
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Creeper.h"
#include "../World.h"
#include "../Entities/ProjectileEntity.h"
#include "../Entities/Player.h"





cCreeper::cCreeper(void) :
	super("Creeper", mtCreeper, "mob.creeper.say", "mob.creeper.say", 0.6, 1.8),
	m_bIsBlowing(false),
	m_bIsCharged(false),
	m_BurnedWithFlintAndSteel(false),
	m_ExplodingTimer(0)
{
}





void cCreeper::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (!ReachedFinalDestination() && !m_BurnedWithFlintAndSteel)
	{
		m_ExplodingTimer = 0;
		m_bIsBlowing = false;
		m_World->BroadcastEntityMetadata(*this);
	}
	else
	{
		if (m_bIsBlowing)
		{
			m_ExplodingTimer += 1;
		}

		if (m_ExplodingTimer == 30)
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

	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER);

	if ((a_Killer != NULL) && a_Killer->IsProjectile() && (((cProjectileEntity *)a_Killer)->GetCreatorUniqueID() >= 0))
	{
		class cProjectileCreatorCallback : public cEntityCallback
		{
		public:
			cProjectileCreatorCallback(void)
			{
			}

			virtual bool Item(cEntity * a_Entity) override
			{
				if (a_Entity->IsMob() && ((cMonster *)a_Entity)->GetMobType() == mtSkeleton)
				{
					return true;
				}
				return false;
			}
		};

		cProjectileCreatorCallback PCC;
		if (GetWorld()->DoWithEntityByID(((cProjectileEntity *)a_Killer)->GetCreatorUniqueID(), PCC))
		{
			// 12 music discs. TickRand starts from 0 to 11. Disk IDs start at 2256, so add that. There.
			AddRandomDropItem(a_Drops, 1, 1, (short)m_World->GetTickRandomNumber(11) + 2256);
		}
	}
}





bool cCreeper::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
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





void cCreeper::Attack(float a_Dt)
{
	UNUSED(a_Dt);

	if (!m_bIsBlowing)
	{
		m_World->BroadcastSoundEffect("game.tnt.primed", GetPosX(), GetPosY(), GetPosZ(), 1.f, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));
		m_bIsBlowing = true;
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cCreeper::OnRightClicked(cPlayer & a_Player)
{
	if ((a_Player.GetEquippedItem().m_ItemType == E_ITEM_FLINT_AND_STEEL))
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.UseEquippedItem();
		}
		m_World->BroadcastSoundEffect("game.tnt.primed", GetPosX(), GetPosY(), GetPosZ(), 1.f, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));
		m_bIsBlowing = true;
		m_World->BroadcastEntityMetadata(*this);
		m_BurnedWithFlintAndSteel = true;
	}
}

