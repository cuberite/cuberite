
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"





cSkeleton::cSkeleton(void) :
	super("Skeleton", 51, "mob.skeleton.hurt", "mob.skeleton.death", 0.6, 1.8)
{
}





void cSkeleton::Tick(float a_Dt, cChunk & a_Chunk)
{
	cMonster::Tick(a_Dt, a_Chunk);

	// TODO Outsource
	// TODO should do SkyLight check, mobs in the dark don´t burn 
	if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && !IsOnFire())
	{
		// Burn for 10 ticks, then decide again
		StartBurning(10);
	}
}





void cSkeleton::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ARROW);
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_BONE);
}




