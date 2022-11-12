
#include "Globals.h"

#include "Endermite.h"

#include "../World.h"
#include "../Chunk.h"
#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockInfested.h"





cEndermite::cEndermite() :
	Super("Endermite", mtEndermite, "entity.endermite.hurt", "entity.endermite.death", "entity.endermite.ambient", 0.4f, 0.3f)
{
}

