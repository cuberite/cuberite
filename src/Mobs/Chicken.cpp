#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chicken.h"
#include "../World.h"





cChicken::cChicken(void) :
	Super("Chicken", mtChicken, "entity.chicken.hurt", "entity.chicken.death", "entity.chicken.ambient", 0.4, 0.7),
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
		Drops.push_back(cItem(E_ITEM_EGG, 1));
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
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_FEATHER);
	AddRandomDropItem(a_Drops, 1, 1, IsOnFire() ? E_ITEM_COOKED_CHICKEN : E_ITEM_RAW_CHICKEN);
}





bool cChicken::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtFalling)
	{
		return false;
	}

	return Super::DoTakeDamage(a_TDI);
}
