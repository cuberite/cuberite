
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bat.h"
#include "../Chunk.h"


cBat::cBat(void) :
	super("Bat", mtBat,  "entity.bat.hurt", "entity.bat.death", 0.5, 0.9)
{
	SetGravity(-2.0f);
	SetAirDrag(0.05f);
}


