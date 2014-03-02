
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
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		
		// FIXME: Do not use cPiston class for dispenser placement!
		a_BlockMeta = cPiston::RotationPitchToMetaData(a_Player->GetYaw(), a_Player->GetPitch());
		return true;
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag.  Lowest three bits are position.  0x08 == 1000
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com) 0x07 == 0111
		switch (a_Meta & 0x07)
		{
			case 0x02: return 0x05 + OtherMeta;  // North -> East
			case 0x05: return 0x03 + OtherMeta;  // East  -> South
			case 0x03: return 0x04 + OtherMeta;  // South -> West
			case 0x04: return 0x02 + OtherMeta;  // West  -> North
		}
		// Facing Up/Down
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag.  Lowest three bits are position.  0x08 == 1000
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com) 0x07 == 0111
		switch (a_Meta & 0x07)
		{
			case 0x02: return 0x04 + OtherMeta;  // North -> West
			case 0x04: return 0x03 + OtherMeta;  // West  -> South
			case 0x03: return 0x05 + OtherMeta;  // South -> East
			case 0x05: return 0x02 + OtherMeta;  // East  -> North
		}
		// Facing Up/Down
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag.  Lowest three bits are position.  0x08 == 1000
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Rotations defined by by a table. (Source, mincraft.gamepedia.com) 0x07 == 0111
		switch (a_Meta & 0x07)
		{
			case 0x02: return 0x03 + OtherMeta;  // North -> South
			case 0x03: return 0x02 + OtherMeta;  // South -> North
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag.  Lowest three bits are position.  0x08 == 1000
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Mirrors defined by by a table. (Source, mincraft.gamepedia.com) 0x07 == 0111
		switch (a_Meta & 0x07)
		{
			case 0x00: return 0x01 + OtherMeta;  // Down -> Up
			case 0x01: return 0x00 + OtherMeta;  // Up   -> Down
		}
		// Not Facing Up or Down; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag.  Lowest three bits are position.  0x08 == 1000
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Mirrors defined by by a table. (Source, mincraft.gamepedia.com) 0x07 == 0111
		switch (a_Meta & 0x07)
		{
			case 0x04: return 0x05 + OtherMeta;  // West -> East
			case 0x05: return 0x04 + OtherMeta;  // East -> West
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
} ;




