
#include "Globals.h"
#include "BlockLever.h"
#include "../Entities/Player.h"
#include "../Simulator/RedstoneSimulator.h"





cBlockLeverHandler::cBlockLeverHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockLeverHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	// Flip the ON bit on/off. Using XOR bitwise operation to turn it on/off.
	NIBBLETYPE Meta = ((a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) ^ 0x08) & 0x0f);

	a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
	if (Meta & 0x08)
	{
		a_World->BroadcastSoundEffect("random.click", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.6f);
	}
	else
	{
		a_World->BroadcastSoundEffect("random.click", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.5f);
	}
}




