
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Bat.h"
#include "../Vector3.h"
#include "../Chunk.h"


cBat::cBat(void) :
	super("Bat", mtBat,  "mob.bat.hurt", "mob.bat.death", 0.5, 0.9)
{
	SetGravity(-2.0f);
	SetAirDrag(0.05f);
}





void cBat::WriteMetadata(cMetadataWriter & a_Writer) const
{
	// super isn't used here because that refers to cPassiveMonster, which contains the is baby flag.
	// Since bat doesn't extend Ageable but does extend insentient, we need to skip the ageable metadata.
	// This is an odd special case that is caused by the differences between vanilla's entity hierarchy and
	// Cuberite's entity hierarchy.
	cMonster::WriteMetadata(a_Writer);
	a_Writer.WriteByte(IsHanging() ? 1 : 0);  // Flags
}


