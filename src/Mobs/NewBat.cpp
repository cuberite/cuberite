
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewBat.h"
#include "../Vector3.h"
#include "../Chunk.h"


cNewBat::cNewBat(void) :
	super("Bat", mtBat,  "mob.bat.hurt", "mob.bat.death", 0.5, 0.9)
{
}


