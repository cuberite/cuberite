
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wither.h"

#include "../World.h"
#include "../Entities/Player.h"





cWither::cWither(void) :
	super("Wither", mtWither, "mob.wither.hurt", "mob.wither.death", 0.9, 4.0)
{
	SetMaxHealth(300);
}





bool cWither::Initialize(cWorld & a_World)
{
	// Set health before BroadcastSpawnEntity()
	SetHealth(GetMaxHealth() / 3);

	return super::Initialize(a_World);
}





void cWither::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 1, 1, E_ITEM_NETHER_STAR);
}




