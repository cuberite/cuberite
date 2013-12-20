
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombie.h"
#include "../World.h"
#include "../LineBlockTracer.h"





cZombie::cZombie(bool a_IsVillagerZombie) :
	super("Zombie", mtZombie, "mob.zombie.hurt", "mob.zombie.death", 0.6, 1.8),
	m_IsVillagerZombie(a_IsVillagerZombie),
	m_IsConverting(false)
{
	SetBurnsInDaylight(true);
}





void cZombie::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ROTTEN_FLESH);
	
	// TODO: Rare drops
}





void cZombie::MoveToPosition(const Vector3f & a_Position)
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




