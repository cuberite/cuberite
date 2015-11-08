
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"
#include "../World.h"
#include "../Entities/ArrowEntity.h"
#include "ClientHandle.h"




cSkeleton::cSkeleton(bool IsWither) :
	super("Skeleton", mtSkeleton, "mob.skeleton.hurt", "mob.skeleton.death", 0.6, 1.8),
	m_bIsWither(IsWither)
{
	SetBurnsInDaylight(true);
}





void cSkeleton::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	if (IsWither())
	{
		AddRandomUncommonDropItem(a_Drops, 33.0f, E_ITEM_COAL);
		cItems RareDrops;
		RareDrops.Add(cItem(E_ITEM_HEAD, 1, 1));
		AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
	}
	else
	{
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_ARROW);

	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_BONE);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}





bool cSkeleton::Attack(std::chrono::milliseconds a_Dt)
{
	cFastRandom Random;
	m_AttackInterval += (static_cast<float>(a_Dt.count()) / 1000) * m_AttackRate;
	if ((m_Target != nullptr) && (m_AttackInterval > 3.0))
	{
		Vector3d Inaccuracy = Vector3d(Random.NextFloat(0.5) - 0.25, Random.NextFloat(0.5) - 0.25, Random.NextFloat(0.5) - 0.25);
		Vector3d Speed = (m_Target->GetPosition() + Inaccuracy - GetPosition()) * 5;
		Speed.y = Speed.y - 1 + Random.NextInt(3);
		cArrowEntity * Arrow = new cArrowEntity(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		if (Arrow == nullptr)
		{
			return false;
		}
		if (!Arrow->Initialize(*m_World))
		{
			delete Arrow;
			Arrow = nullptr;
			return false;
		}
		m_World->BroadcastSpawnEntity(*Arrow);
		m_AttackInterval = 0.0;
		
		return true;
	}
	return false;
}





void cSkeleton::SpawnOn(cClientHandle & a_ClientHandle)
{
	super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendEntityEquipment(*this, 0, cItem(E_ITEM_BOW));
}




