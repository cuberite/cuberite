#include "Globals.h"
#include "BlockRedstoneRepeater.h"
#include "../Item.h"
#include "../World.h"
#include "../Redstone.h"
#include "../Player.h"

cBlockRedstoneRepeaterHandler::cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}

void cBlockRedstoneRepeaterHandler::OnPlaced(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir)
{
	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_BlockX, a_BlockY, a_BlockZ, false);
}

void cBlockRedstoneRepeaterHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_BlockX, a_BlockY, a_BlockZ, false);
}

void cBlockRedstoneRepeaterHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, ((a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) + 0x04) & 0x0f));
}

void cBlockRedstoneRepeaterHandler::OnDigging(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	OnUse(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
}

void cBlockRedstoneRepeaterHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cRedstone::RepeaterRotationToMetaData(a_Player->GetRotation()));
	OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
}