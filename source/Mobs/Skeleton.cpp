
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"
#include "../World.h"
#include "../Entities/ProjectileEntity.h"
#include "../Entities/Entity.h"





cSkeleton::cSkeleton(bool IsWither) :
	super("Skeleton", mtSkeleton, "mob.skeleton.hurt", "mob.skeleton.death", 0.6, 1.8),
	m_bIsWither(IsWither)
{
	SetBurnsInDaylight(true);
}





void cSkeleton::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ARROW);
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_BONE);
}





void cSkeleton::MoveToPosition(const Vector3f & a_Position)
{
	m_Destination = a_Position;

	// If the destination is in the sun and if it is not night AND the skeleton isn't on fire then block the movement.
	if ((m_World->GetBlockSkyLight((int) a_Position.x, (int) a_Position.y, (int) a_Position.z) == 15) && (m_World->GetTimeOfDay() < 13187) && !IsOnFire())
	{
		m_bMovingToDestination = false;
		return;
	}
	m_bMovingToDestination = true;
}