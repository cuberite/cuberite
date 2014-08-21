
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Creeper.h"
#include "../World.h"
#include "../Entities/ProjectileEntity.h"
#include "../Entities/Player.h"
#include "../Entities/Entity.h"





cCreeper::cCreeper(void) :
	super("Creeper", mtCreeper, "mob.creeper.say", "mob.creeper.say", 0.6, 1.8),
	m_bIsBlowing(false),
	m_bIsCharged(false),
	// m_BurnedWithFlintAndSteel(false),
	m_ExplodingTimer(0)
{
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

