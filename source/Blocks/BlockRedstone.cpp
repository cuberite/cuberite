
#include "Globals.h"
#include "BlockRedstone.h"
#include "../Item.h"
#include "../World.h"
#include "../Torch.h"





cBlockRedstoneHandler::cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockRedstoneHandler::OnPlaced(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir)
{
	// Nothing needed yet
}





void cBlockRedstoneHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Nothing needed yet
}





void cBlockRedstoneHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	switch(m_BlockType)
	{
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		{
			a_BlockMeta = cTorch::DirectionToMetaData(a_Dir);
			break;
		}
	}
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, a_BlockMeta);
	OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
}




