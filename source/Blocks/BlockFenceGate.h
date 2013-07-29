
#pragma once

#include "BlockHandler.h"
#include "../Doors.h"





class cBlockFenceGateHandler :
	public cBlockHandler
{
public:
	cBlockFenceGateHandler(BLOCKTYPE a_BlockType) :
		cBlockHandler(a_BlockType)
	{
	}


	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = cDoors::RotationToMetaData(a_Player->GetRotation() + 270);
		return true;
	}


	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE OldMetaData = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		NIBBLETYPE NewMetaData = cDoors::RotationToMetaData(a_Player->GetRotation() + 270);
		OldMetaData ^= 4;  // Toggle the gate
		if ((OldMetaData & 1) == (NewMetaData & 1))
		{
			// Standing in front of the gate - apply new direction
			a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, (OldMetaData & 4) | (NewMetaData & 3));
		}
		else
		{
			// Standing aside - use last direction
			a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, OldMetaData);
		}
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
} ;




