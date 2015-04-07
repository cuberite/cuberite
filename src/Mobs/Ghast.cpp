
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ghast.h"
#include "../World.h"
#include "../Entities/GhastFireballEntity.h"




cGhast::cGhast(void) :
	super("Ghast", mtGhast, "mob.ghast.scream", "mob.ghast.death", 4, 4)
{
}





void cGhast::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GHAST_TEAR);
}





void cGhast::Attack(std::chrono::milliseconds a_Dt)
{
	m_AttackInterval += (static_cast<float>(a_Dt.count()) / 1000) * m_AttackRate;
	
	if ((m_Target != nullptr) && (m_AttackInterval > 3.0))
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;
		cGhastFireballEntity * GhastBall = new cGhastFireballEntity(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		if (GhastBall == nullptr)
		{
			return;
		}
		if (!GhastBall->Initialize(*m_World))
		{
			delete GhastBall;
			GhastBall = nullptr;
			return;
		}
		m_World->BroadcastSpawnEntity(*GhastBall);
		m_AttackInterval = 0.0;
	}
}



