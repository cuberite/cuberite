#include "Globals.h"
#include "BlockVine.h"
#include "../cItem.h"
#include "../cVine.h"
#include "../cPlayer.h"
#include "../cWorld.h"

cBlockVineHandler::cBlockVineHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockVineHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
{
	a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cVine::DirectionToMetaData(a_Dir));
	OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
}