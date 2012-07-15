#include "Globals.h"
#include "BlockFire.h"
#include "../cItem.h"
#include "../cWorld.h"

cBlockFireHandler::cBlockFireHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockFireHandler::OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z)
{
	a_World->DigBlock(a_X, a_Y, a_Z);
}

char cBlockFireHandler::GetDropCount()
{
	return -1;
}

bool cBlockFireHandler::IsClickedThrough()
{
	return true;
}