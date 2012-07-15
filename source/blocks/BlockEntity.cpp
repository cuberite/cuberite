#include "Globals.h"
#include "BlockEntity.h"
#include "../cItem.h"
#include "../cPlayer.h"
#include "../cWorld.h"

cBlockEntityHandler::cBlockEntityHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockEntityHandler::OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z)
{
	a_World->UseBlockEntity(a_Player, a_X, a_Y, a_Z);
}
