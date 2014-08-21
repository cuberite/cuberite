
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewEnderDragon.h"





cNewEnderDragon::cNewEnderDragon(void) :
	// TODO: Vanilla source says this, but is it right? Dragons fly, they don't stand
	super("EnderDragon", mtEnderDragon, "mob.enderdragon.hit", "mob.enderdragon.end", 16.0, 8.0)
{
}





void cNewEnderDragon::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	return;
}




