
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WitherSkeleton.h"
#include "../World.h"
#include "../ClientHandle.h"




cWitherSkeleton::cWitherSkeleton(void) :
	Super("WitherSkeleton", mtWitherSkeleton, "entity.wither_skeleton.hurt", "entity.wither_skeleton.death", "entity.wither_skeleton.ambient", 0.7, 2.4)
{
}





bool cWitherSkeleton::Attack(std::chrono::milliseconds a_Dt)
{
	if (GetTarget() == nullptr)
	{
		return false;
	}

	GetTarget()->AddEntityEffect(cEntityEffect::effWither, 200, 0);
	return Super::Attack(a_Dt);
}





void cWitherSkeleton::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomUncommonDropItem(a_Drops, 33.0f, E_ITEM_COAL);
	AddRandomUncommonDropItem(a_Drops, 8.5f, E_ITEM_STONE_SWORD, GetRandomProvider().RandInt<short>(50));

	cItems RareDrops;
	RareDrops.Add(cItem(E_ITEM_HEAD, 1, 1));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);

	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_BONE);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}





void cWitherSkeleton::SpawnOn(cClientHandle & a_ClientHandle)
{
	Super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendEntityEquipment(*this, 0, cItem(E_ITEM_STONE_SWORD));
}
