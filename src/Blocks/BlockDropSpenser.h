
// BlockDropSpenser.h

// Declares the cBlockDropSpenserHandler class representing the BlockHandler for Dropper and Dispenser blocks

#pragma once

#include "../Piston.h"





class cBlockDropSpenserHandler :
	public cBlockEntityHandler
{
public:
	cBlockDropSpenserHandler(BLOCKTYPE a_BlockType) :
		cBlockEntityHandler(a_BlockType)
	{
	}
	

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface * a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		
		// FIXME: Do not use cPiston class for dispenser placement!
		a_BlockMeta = cPiston::RotationPitchToMetaData(a_Player->GetYaw(), a_Player->GetPitch());
		return true;
	}
} ;




