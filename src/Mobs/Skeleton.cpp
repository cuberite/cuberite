
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"
#include "../World.h"
#include "../Entities/ArrowEntity.h"
#include "../ClientHandle.h"




cSkeleton::cSkeleton(void) :
	Super("Skeleton", mtSkeleton, "entity.skeleton.hurt", "entity.skeleton.death", "entity.skeleton.ambient", 0.6f, 1.99f),
	m_ChargingBow(false)
{
}





void cSkeleton::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, Item::Arrow);

	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, Item::Bone);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}





void cSkeleton::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);

	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_ChargingBow && (m_EMState == IDLE))
	{
		// releasing bow if no more target is found
		m_ChargingBow = false;
		m_World->BroadcastEntityMetadata(*this);
	}
}





bool cSkeleton::Attack(std::chrono::milliseconds a_Dt)
{
	StopMovingToPosition();  // Todo handle this in a better way, the skeleton does some uneeded recalcs due to inStateChasing
	auto & Random = GetRandomProvider();

	if (!m_ChargingBow)
	{
		// updating pulling animation
		m_ChargingBow = true;
		m_World->BroadcastEntityMetadata(*this);
	}

	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0))
	{
		Vector3d Inaccuracy = Vector3d(Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25));
		Vector3d Speed = (GetTarget()->GetPosition() + Inaccuracy - GetPosition()) * 5;
		Speed.y += Random.RandInt(-1, 1);

		auto Arrow = std::make_unique<cArrowEntity>(this, GetPosition().addedY(1), Speed);
		auto ArrowPtr = Arrow.get();
		if (!ArrowPtr->Initialize(std::move(Arrow), *m_World))
		{
			return false;
		}

		// releasing bow after arrow was shot
		m_ChargingBow = false;
		m_World->BroadcastEntityMetadata(*this);

		ResetAttackCooldown();
		return true;
	}
	return false;
}





void cSkeleton::SpawnOn(cClientHandle & a_ClientHandle)
{
	Super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendEntityEquipment(*this, 0, cItem(Item::Bow));
}
