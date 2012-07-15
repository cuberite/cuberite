#include "Globals.h"
#include "BlockTorch.h"
#include "../cTorch.h"
#include "../cWorld.h"

cBlockTorchHandler::cBlockTorchHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockTorchHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
{
	a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cTorch::DirectionToMetaData(a_Dir));
	OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
}