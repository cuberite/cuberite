
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Skeleton.h"
#include "../World.h"





cSkeleton::cSkeleton(void) :
	super("Skeleton", 51, "mob.skeleton.hurt", "mob.skeleton.death", 0.6, 1.8)
{
}





void cSkeleton::Tick(float a_Dt, cChunk & a_Chunk)
{
	cMonster::Tick(a_Dt, a_Chunk);

	if ((GetWorld()->GetBlockSkyLight(GetPosX(), GetPosY(), GetPosZ()) == 15) && (GetWorld()->GetBlock(GetPosX(), GetPosY(), GetPosZ()) != E_BLOCK_SOULSAND))
	{
		if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && !IsOnFire())
		{
			// Burn for 100 ticks, then decide again
			StartBurning(100);
		}
	}
}





void cSkeleton::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_ARROW);
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_BONE);
}




