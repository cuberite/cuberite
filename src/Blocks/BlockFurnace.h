
#pragma once

#include "BlockEntity.h"
#include "../World.h"
#include "../Piston.h"





class cBlockFurnaceHandler :
	public cBlockEntityHandler
{
public:
	cBlockFurnaceHandler(BLOCKTYPE a_BlockType) :
		cBlockEntityHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_FURNACE, 1, 0));
	}

	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		
		// FIXME: Do not use cPiston class for furnace placement!
		a_BlockMeta = cPiston::RotationPitchToMetaData(a_Player->GetYaw(), 0);
		
		return true;
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x05;  // North -> East
		case 0x05: return 0x03;  // East  -> South
		case 0x03: return 0x04;  // South -> West
		case 0x04: return 0x02;  // West  -> North
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x04;  // North -> West
		case 0x04: return 0x03;  // West  -> South
		case 0x03: return 0x05;  // South -> East
		case 0x05: return 0x02;  // East  -> North
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com)
		switch (a_Meta)
		{
		case 0x02: return 0x03;  // North -> South
		case 0x03: return 0x02;  // South -> North
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Mirrors over YZ-plane (East/West).
		switch (a_Meta)
		{
		case 0x04: return 0x05;  // West -> East
		case 0x05: return 0x04;  // East -> West
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
} ;




