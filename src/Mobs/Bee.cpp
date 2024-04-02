
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bee.h"
#include "../Chunk.h"





cBee::cBee(void) :
	Super("Bee", mtBee, "entity.bee.hurt", "entity.bee.death", "entity.bee.ambient", 0.5f, 0.9f)
{
	SetMaxHealth(10.0f);
	SetGravity(-2.0f);
	SetAirDrag(0.05f);
}




