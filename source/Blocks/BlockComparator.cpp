
#include "Globals.h"
#include "BlockComparator.h"
#include "../Item.h"
#include "../World.h"
#include "../Simulator/RedstoneSimulator.h"
#include "../Entities/Player.h"





cBlockComparatorHandler::cBlockComparatorHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockComparatorHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Nothing needed yet
}





void cBlockComparatorHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	Meta ^= 0x04; // Toggle 3rd (addition/subtraction) bit with XOR
	a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
}




bool cBlockComparatorHandler::GetPlacementBlockTypeMeta(
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




