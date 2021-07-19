#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chicken.h"
#include "../World.h"





cChicken::cChicken(void) :
	Super("Chicken", mtChicken, "entity.chicken.hurt", "entity.chicken.death", "entity.chicken.ambient", 0.4f, 0.7f),
	m_EggDropTimer(0)
{
	SetGravity(-2.0f);
	SetAirDrag(0.0f);
}





void cChicken::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (IsBaby())
	{
		return;  // Babies don't lay eggs
	}

	if (
		((m_EggDropTimer == 6000) && GetRandomProvider().RandBool()) ||
		m_EggDropTimer == 12000
	)
	{
		cItems Drops;
		m_EggDropTimer = 0;
		Drops.emplace_back(Item::Egg);
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
	}
	else
	{
		m_EggDropTimer++;
	}
}





void cChicken::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (IsBaby())
	{
		return;  // Babies don't drop items
	}

	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, Item::Feather);
	AddRandomDropItem(a_Drops, 1, 1, IsOnFire() ? Item::CookedChicken : Item::Chicken);
}





bool cChicken::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtFalling)
	{
		return false;
	}

	return Super::DoTakeDamage(a_TDI);
}
