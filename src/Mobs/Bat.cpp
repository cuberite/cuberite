
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bat.h"
#include "../Vector3d.h"
#include "../Chunk.h"


cBat::cBat(void) :
		// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
	super("Bat", mtBat,  "mob.bat.hurt", "mob.bat.death", 0.7, 0.7)
{
}


