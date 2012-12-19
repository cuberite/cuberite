
#include "Globals.h"
#include "BlockDoor.h"
#include "../Item.h"
#include "../World.h"
#include "../Doors.h"
#include "../Player.h"





cBlockDoorHandler::cBlockDoorHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockDoorHandler::OnPlaced(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir)
{

}





void cBlockDoorHandler::OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	char OldMeta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	if (OldMeta & 8)
	{
		// Was upper part of door
		if (cDoors::IsDoor(a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ)))
		{
			a_World->FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was lower part
		if (cDoors::IsDoor(a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ)))
		{
			a_World->FastSetBlock(a_BlockX, a_BlockY + 1, a_BlockZ, E_BLOCK_AIR, 0);
		}
	}
}





void cBlockDoorHandler::OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cDoors::ChangeDoor(a_World, a_BlockX, a_BlockY, a_BlockZ);
}





void cBlockDoorHandler::PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	if (a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ) == E_BLOCK_AIR)
	{
		a_BlockMeta = cDoors::RotationToMetaData(a_Player->GetRotation());
		char a_TopBlockMeta = 8;
		if( (a_BlockMeta == 0) && (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1) == m_BlockType) ||
			(a_BlockMeta == 1) && (a_World->GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ) == m_BlockType) ||
			(a_BlockMeta == 2) && (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1) == m_BlockType) ||
			(a_BlockMeta == 3) && (a_World->GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ) == m_BlockType))
		{
			a_TopBlockMeta = 9;
		}
		a_World->SetBlock(a_BlockX, a_BlockY + 1, a_BlockZ, m_BlockType, a_TopBlockMeta);
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, a_BlockMeta);
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}
}





const char * cBlockDoorHandler::GetStepSound(void)
{
	return (m_BlockType == E_BLOCK_WOODEN_DOOR) ? "step.wood" : "step.stone";
}




