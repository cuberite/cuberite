
#include "Globals.h"
#include "BlockRedstone.h"
#include "../Item.h"
#include "../World.h"
#include "../Torch.h"





cBlockRedstoneHandler::cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockRedstoneHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Nothing needed yet
}




