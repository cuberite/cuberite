
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ghast.h"
#include "../World.h"
#include "../Entities/GhastFireballEntity.h"




cGhast::cGhast(void) :
	super("Ghast", mtGhast, "entity.ghast.hurt", "entity.ghast.death", 4, 4)
{
}





void cGhast::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GHAST_TEAR);
}





bool cGhast::Attack(std::chrono::milliseconds a_Dt)
{
	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0))
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;

		auto GhastBall = cpp14::make_unique<cGhastFireballEntity>(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		auto GhastBallPtr = GhastBall.get();
		if (!GhastBallPtr->Initialize(std::move(GhastBall), *m_World))
		{
			return false;
		}

		ResetAttackCooldown();
		return true;
	}
	return false;
}



