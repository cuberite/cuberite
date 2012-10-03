#include "Globals.h"
#include "BlockRedstone.h"
#include "../Item.h"
#include "../World.h"
#include "../Redstone.h"
#include "../Torch.h"

cBlockRedstoneHandler::cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}

void cBlockRedstoneHandler::OnPlaced(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir)
{
	cRedstone Redstone(a_World);
	bool Added = false;
	if(a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_REDSTONE_TORCH_ON)
		Added = true;

	Redstone.ChangeRedstone(a_BlockX, a_BlockY, a_BlockZ, Added);
}

void cBlockRedstoneHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_BlockX, a_BlockY, a_BlockZ, false);
}

void cBlockRedstoneHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	switch(m_BlockType)
	{
	case E_BLOCK_REDSTONE_TORCH_ON:
	case E_BLOCK_REDSTONE_TORCH_OFF:
		a_BlockMeta = cTorch::DirectionToMetaData(a_Dir);
		break;
	
	}
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, a_BlockMeta);
	OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
}