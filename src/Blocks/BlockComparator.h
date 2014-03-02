
#pragma once

#include "BlockHandler.h"
#include "BlockRedstoneRepeater.h"





class cBlockComparatorHandler :
	public cBlockHandler
{
public:
	cBlockComparatorHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		Meta ^= 0x04; // Toggle 3rd (addition/subtraction) bit with XOR
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_ITEM_COMPARATOR, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR));
	}
	

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
		) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = cBlockRedstoneRepeaterHandler::RepeaterRotationToMetaData(a_Player->GetYaw());
		return true;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Direction is stored in least two significant bits.  0x0C == 1100.
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Rotates according to table; 0x03 == 0011.
		switch (a_Meta & 0x03)
		{
			case 0x00: return 0x03 + OtherMeta;  // North -> West
			case 0x03: return 0x02 + OtherMeta;  // West  -> South
			case 0x02: return 0x01 + OtherMeta;  // South -> East
			case 0x01: return 0x00 + OtherMeta;  // East  -> South
		}
		// To avoid a compiler warning;
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Direction is stored in least two significant bits.  0x0C == 1100.
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Rotates according to table; 0x03 == 0011.
		switch (a_Meta & 0x03)
		{
			case 0x00: return 0x01 + OtherMeta;  // North -> East
			case 0x01: return 0x02 + OtherMeta;  // East  -> South
			case 0x02: return 0x03 + OtherMeta;  // South -> West
			case 0x03: return 0x00 + OtherMeta;  // West  -> South
		}
		// To avoid a compiler warning;
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Direction is stored in least two significant bits.  0x0C == 1100.
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Mirrors according to table; 0x03 == 0011.
		switch (a_Meta & 0x03)
		{
			case 0x00: return 0x02 + OtherMeta;  // North -> South
			case 0x02: return 0x00 + OtherMeta;  // South -> South
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Direction is stored in least two significant bits.  0x0C == 1100.
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Mirrors according to table; 0x03 == 0011.
		switch (a_Meta & 0x03)
		{
			case 0x01: return 0x03 + OtherMeta;  // East  -> West
			case 0x03: return 0x01 + OtherMeta;  // West  -> East
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}
} ;




