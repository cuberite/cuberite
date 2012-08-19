#include "Globals.h"
#include "BlockRedstoneRepeater.h"
#include "../cItem.h"
#include "../cWorld.h"
#include "../cRedstone.h"
#include "../cPlayer.h"

cBlockRedstoneRepeaterHandler::cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockRedstoneRepeaterHandler::OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir)
{
	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_X, a_Y, a_Z, false);
}

void cBlockRedstoneRepeaterHandler::OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_X, a_Y, a_Z, false);
}

void cBlockRedstoneRepeaterHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z)
{
	a_World->FastSetBlock(a_X, a_Y, a_Z, m_BlockID, ((a_World->GetBlockMeta(a_X, a_Y, a_Z) + 0x04) & 0x0f));
}

void cBlockRedstoneRepeaterHandler::OnDigging(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z)
{
	OnUse(a_World, a_Player, a_X, a_Y, a_Z);
}

void cBlockRedstoneRepeaterHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
{
	a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cRedstone::RepeaterRotationToMetaData(a_Player->GetRotation()));
	OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
}