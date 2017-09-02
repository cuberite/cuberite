
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Creeper.h"
#include "../World.h"
#include "../Entities/ProjectileEntity.h"
#include "../Entities/Player.h"





cCreeper::cCreeper(void) :
	super(mtCreeper, "entity.creeper.hurt", "entity.creeper.death", 0.6, 1.8)
{
	m_EMPersonality = AGGRESSIVE;
	GetMonsterConfig("Creeper");
}





void cCreeper::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (IsBlowing())
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
		((reinterpret_cast<cProjectileEntity *>(a_Killer))->GetCreatorUniqueID() != cEntity::INVALID_ID))
	{
		class cProjectileCreatorCallback : public cEntityCallback
		{
		public:
			cProjectileCreatorCallback(void) {}

			virtual bool Item(cEntity * a_Entity) override
			{
				if (a_Entity->IsMob() && ((reinterpret_cast<cMonster *>(a_Entity))->GetMobType() == mtSkeleton))
				{
					return true;
				}
				return false;
			}
		} PCC;
		if (GetWorld()->DoWithEntityByID((reinterpret_cast<cProjectileEntity *>(a_Killer))->GetCreatorUniqueID(), PCC))
		{
			AddRandomDropItem(a_Drops, 1, 1, static_cast<short>(m_World->GetTickRandomNumber(11) + E_ITEM_FIRST_DISC));
		}
	}
}





bool cCreeper::IsBlowing(void) const
{
	return m_BehaviorSuicideBomber.IsBlowing();
}





bool cCreeper::IsCharged(void) const
{
	return m_BehaviorSuicideBomber.IsCharged();
}





bool cCreeper::IsBurnedWithFlintAndSteel(void) const
{
	return m_BehaviorSuicideBomber.IsBurnedWithFlintAndSteel();
}
