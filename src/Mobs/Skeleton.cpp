
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"
#include "../World.h"




cSkeleton::cSkeleton(bool IsWither) :
	super("Skeleton", mtSkeleton, "mob.skeleton.hurt", "mob.skeleton.death", 0.6, 1.8),
	m_bIsWither(IsWither)
{
	SetBurnsInDaylight(true);
}





void cSkeleton::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(E_ENCHANTMENT_LOOTING);
	}
	if (IsWither())
	{
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_BONE);
		AddRandomUncommonDropItem(a_Drops, 33.0f, E_ITEM_COAL);
		cItems RareDrops;
		RareDrops.Add(cItem(E_ITEM_HEAD, 1, 1));
		AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
		AddRandomArmorDropItem(a_Drops, LootingLevel);
	}
	else
	{
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_ARROW);
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_BONE);
		AddRandomArmorDropItem(a_Drops, LootingLevel);
	}
}





void cSkeleton::MoveToPosition(const Vector3f & a_Position)
{
	// If the destination is in the sun and if it is not night AND the skeleton isn't on fire then block the movement.
	if (
		!IsOnFire() &&
		(m_World->GetTimeOfDay() < 13187) &&
		(m_World->GetBlockSkyLight((int) a_Position.x, (int) a_Position.y, (int) a_Position.z) == 15)
		)
	{
		m_bMovingToDestination = false;
		return;
	}

	super::MoveToPosition(a_Position);
}





void cSkeleton::Attack(float a_Dt)
{
	m_AttackInterval += a_Dt * m_AttackRate;

	if (m_Target != NULL && m_AttackInterval > 3.0)
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;
		cArrowEntity * Arrow = new cArrowEntity(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		if (Arrow == NULL)
		{
			return;
		}
		if (!Arrow->Initialize(m_World))
		{
			delete Arrow;
			return;
		}
		m_World->BroadcastSpawnEntity(*Arrow);
		m_AttackInterval = 0.0;
	}
}