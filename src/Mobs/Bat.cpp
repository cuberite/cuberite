
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bat.h"
#include "../Vector3.h"
#include "../Chunk.h"


cBat::cBat(CreateMonsterInfo a_Info) :
	super(a_Info, "Bat", mtBat,  "mob.bat.hurt", "mob.bat.death", 0.5, 0.9)
{
}


