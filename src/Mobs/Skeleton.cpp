
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"
#include "../World.h"
#include "../Entities/ArrowEntity.h"
#include "ClientHandle.h"




cSkeleton::cSkeleton(bool IsWither) :
	super("Skeleton", mtSkeleton, "entity.skeleton.hurt", "entity.skeleton.death", 0.6, 1.8),
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
	StopMovingToPosition();  // Todo handle this in a better way, the skeleton does some uneeded recalcs due to inStateChasing
	auto & Random = GetRandomProvider();
	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0))
	{
		Vector3d Inaccuracy = Vector3d(Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25));
		Vector3d Speed = (GetTarget()->GetPosition() + Inaccuracy - GetPosition()) * 5;
		Speed.y += Random.RandInt(-1, 1);

		auto Arrow = cpp14::make_unique<cArrowEntity>(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		auto ArrowPtr = Arrow.get();
		if (!ArrowPtr->Initialize(std::move(Arrow), *m_World))
		{
			return false;
		}

		ResetAttackCooldown();
		return true;
	}
	return false;
}





void cSkeleton::SpawnOn(cClientHandle & a_ClientHandle)
{
	super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendEntityEquipment(*this, 0, cItem(E_ITEM_BOW));
}




