#include "Globals.h"
#include "BlockRedstone.h"
#include "../cItem.h"
#include "../cWorld.h"
#include "../cRedstone.h"
#include "../cTorch.h"

cBlockRedstoneHandler::cBlockRedstoneHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockRedstoneHandler::OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir)
{
	cRedstone Redstone(a_World);
	bool Added = false;
	if(a_World->GetBlock(a_X, a_Y, a_Z) == E_BLOCK_REDSTONE_TORCH_ON)
		Added = true;

	Redstone.ChangeRedstone(a_X, a_Y, a_Z, Added);
}

void cBlockRedstoneHandler::OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_X, a_Y, a_Z, false);
}

void cBlockRedstoneHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
{
	switch(m_BlockID)
	{
	case E_BLOCK_REDSTONE_TORCH_ON:
	case E_BLOCK_REDSTONE_TORCH_OFF:
		a_BlockMeta = cTorch::DirectionToMetaData(a_Dir);
		break;
	
	}
	a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, a_BlockMeta);
	OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
}