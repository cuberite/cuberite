
#include "Globals.h"
#include "BlockRedstoneRepeater.h"
#include "../Item.h"
#include "../World.h"
#include "../Simulator/RedstoneSimulator.h"
#include "../Entities/Player.h"





cBlockRedstoneRepeaterHandler::cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockRedstoneRepeaterHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Nothing needed yet
}





void cBlockRedstoneRepeaterHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, ((a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) + 0x04) & 0x0f));
}





void cBlockRedstoneRepeaterHandler::OnDigging(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	OnUse(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_NONE, 8, 8, 8);
}




bool cBlockRedstoneRepeaterHandler::GetPlacementBlockTypeMeta(
	cWorld * a_World, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	a_BlockType = m_BlockType;
	a_BlockMeta = cRedstoneSimulator::RepeaterRotationToMetaData(a_Player->GetRotation());
	return true;
}




