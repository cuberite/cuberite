
#include "Globals.h"
#include "BlockLever.h"
#include "../Item.h"
#include "../World.h"
#include "../Player.h"
#include "../Simulator/RedstoneSimulator.h"





cBlockLeverHandler::cBlockLeverHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockLeverHandler::OnPlaced(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir)
{
	// Noting needed yet
}





void cBlockLeverHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Nothing needed yet
}





void cBlockLeverHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	//Flip the ON bit on/off. Using XOR bitwise operation to turn it on/off.
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, ((a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) ^ 0x08) & 0x0f));
}





void cBlockLeverHandler::OnDigging(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	OnUse(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
}





void cBlockLeverHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cRedstoneSimulator::LeverDirectionToMetaData(a_Dir));
	OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
}




