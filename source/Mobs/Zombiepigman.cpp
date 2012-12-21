
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombiepigman.h"





cZombiepigman::cZombiepigman()
{
	m_MobType = 57;
	GetMonsterConfig("Zombiepigman");
}





bool cZombiepigman::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cZombiepigman") == 0) || super::IsA(a_EntityType));
}





void cZombiepigman::Tick(float a_Dt)
{
	super::Tick(a_Dt);

	// TODO Same as noticed in cSkeleton AND Do they really burn by sun?? :D In the neather is no sun :D
	if ((GetWorld()->GetTimeOfDay() < (12000 + 1000)) && (GetMetaData() != BURNING))
	{
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}





void cZombiepigman::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_ROTTEN_FLESH);
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_GOLD_NUGGET);
	
	// TODO: Rare drops
}





void cZombiepigman::KilledBy(cPawn * a_Killer)
{
	super::KilledBy(a_Killer);

	if ((a_Killer != NULL) && (a_Killer->GetEntityType() == eEntityType_Player))
	{
		// TODO: Anger all nearby zombie pigmen
		// TODO: In vanilla, if one player angers ZPs, do they attack any nearby player, or only that one attacker?
	}
}




