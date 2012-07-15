#include "Globals.h"
#include "BlockDoor.h"
#include "../cItem.h"
#include "../cWorld.h"
#include "../cDoors.h"
#include "../cPlayer.h"


cBlockDoorHandler::cBlockDoorHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockDoorHandler::OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir)
{

}

void cBlockDoorHandler::OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	char OldMeta = a_World->GetBlockMeta(a_X, a_Y, a_Z);

	if (OldMeta & 8)
	{
		// Was upper part of door
		if (cDoors::IsDoor(a_World->GetBlock(a_X, a_Y - 1, a_Z)))
		{
			a_World->FastSetBlock(a_X, a_Y - 1, a_Z, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was lower part
		if (cDoors::IsDoor(a_World->GetBlock(a_X, a_Y + 1, a_Z)))
		{
			a_World->FastSetBlock(a_X, a_Y + 1, a_Z, E_BLOCK_AIR, 0);
		}
	}
}

void cBlockDoorHandler::OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z)
{
	cDoors::ChangeDoor(a_World, a_X, a_Y, a_Z);
}

char cBlockDoorHandler::GetDropCount()
{
	return 1;
}

void cBlockDoorHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
{
	if (a_World->GetBlock(a_X, a_Y + 1, a_Z) == E_BLOCK_AIR)
	{
		a_BlockMeta = cDoors::RotationToMetaData(a_Player->GetRotation());
		a_World->SetBlock(a_X, a_Y + 1, a_Z, m_BlockID, a_BlockMeta + 8);
		a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, a_BlockMeta);
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}
}