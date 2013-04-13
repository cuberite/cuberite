
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombie.h"





cZombie::cZombie(void) :
	super("Zombie", 54, "mob.zombie.hurt", "mob.zombie.death")
{
}





void cZombie::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	// TODO Same as in cSkeleton :D
	if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && !IsBurning())
	{
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}





void cZombie::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ROTTEN_FLESH);
	
	// TODO: Rare drops
}




