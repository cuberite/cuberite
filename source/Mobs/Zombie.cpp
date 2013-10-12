
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombie.h"
#include "../World.h"
#include "../LineBlockTracer.h"




cZombie::cZombie(bool IsVillagerZombie) :
	super("Zombie", 54, "mob.zombie.hurt", "mob.zombie.death", 0.6, 1.8),
	m_bIsConverting(false),
	m_bIsVillagerZombie(IsVillagerZombie)
{
	SetBurnsInDaylight(true);
}





void cZombie::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ROTTEN_FLESH);
	
	// TODO: Rare drops
}




