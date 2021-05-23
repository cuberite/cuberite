
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WitherSkeleton.h"
#include "../World.h"
#include "../ClientHandle.h"




cWitherSkeleton::cWitherSkeleton(void) :
	Super("WitherSkeleton", mtWitherSkeleton, "entity.wither_skeleton.hurt", "entity.wither_skeleton.death", "entity.wither_skeleton.ambient", 0.7f, 2.4f)
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
	AddRandomUncommonDropItem(a_Drops, 33.0f, Item::Coal);
	AddRandomUncommonDropItem(a_Drops, 8.5f, Item::StoneSword);  // TODO(12xx12) readd this when move to new item enum is done;, GetRandomProvider().RandInt<short>(50));

	cItems RareDrops;
	RareDrops.Add(cItem(Item::WitherSkeletonSkull));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);

	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, Item::Bone);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}





void cWitherSkeleton::SpawnOn(cClientHandle & a_ClientHandle)
{
	Super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendEntityEquipment(*this, 0, cItem(Item::StoneSword));
}
