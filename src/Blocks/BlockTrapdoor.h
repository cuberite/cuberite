
#pragma once

#include "BlockHandler.h"





class cBlockTrapdoorHandler :
	public cBlockHandler
{
public:
	cBlockTrapdoorHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		// Flip the ON bit on/off using the XOR bitwise operation
		NIBBLETYPE Meta = (a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) ^ 0x04);

		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
		) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = BlockFaceToMetaData(a_BlockFace);

		if (a_CursorY > 7)
		{
			a_BlockMeta |= 0x8;
		}
		return true;
	}
	
	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_ZP: return 0x1;
			case BLOCK_FACE_ZM: return 0x0;
			case BLOCK_FACE_XP: return 0x3;
			case BLOCK_FACE_XM: return 0x2;
			default:
			{
				ASSERT(!"Unhandled block face!");
				return 0;
			}
		}
	}

	inline static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x3)
		{
			case 0x0: return BLOCK_FACE_ZM;
			case 0x1: return BLOCK_FACE_ZP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_XP;
			default:
			{
				ASSERT(!"Unhandled block meta!");
				return BLOCK_FACE_NONE;
			}
		}
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		NIBBLETYPE Meta;
		a_Chunk.UnboundedRelGetBlockMeta(a_RelX, a_RelY, a_RelZ, Meta);

		AddFaceDirection(a_RelX, a_RelY, a_RelZ, BlockMetaDataToBlockFace(Meta), true);
		BLOCKTYPE BlockIsOn; a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOn);

		return (a_RelY > 0) && cBlockInfo::IsSolid(BlockIsOn);
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Bits 0x08 and 0x04 (Bits #3 and #4) are flags; lowest two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Rotates by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
		switch (a_Meta & 0x03)
		{
			case 0x00: return 0x02 + OtherMeta; // South -> East
			case 0x02: return 0x01 + OtherMeta; // East  -> North
			case 0x01: return 0x03 + OtherMeta; // South -> West
			case 0x03: return 0x00 + OtherMeta; // West  -> South
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Bits 0x08 and 0x04 (Bits #3 and #4) are flags; lowest two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Rotates by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
		switch (a_Meta & 0x03)
		{
			case 0x00: return 0x03 + OtherMeta; // South -> West
			case 0x03: return 0x01 + OtherMeta; // West  -> North
			case 0x01: return 0x02 + OtherMeta; // South -> East
			case 0x02: return 0x00 + OtherMeta; // East  -> South
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Bits 0x08 and 0x04 (Bits #3 and #4) are flags; lowest two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Mirrors by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
		switch (a_Meta & 0x03)
		{
			case 0x01: return 0x00 + OtherMeta; // North -> South
			case 0x00: return 0x01 + OtherMeta; // South -> North
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Bit 0x08 is a flag that determines if the trap door is on the top or bottom of a block.  0x07 == 0111
		NIBBLETYPE OtherMeta = a_Meta & 0x07;
		// Mirrors by returning appropate metavalue; values are determined by a Metadata table. 0x08 == 1000
		switch (a_Meta & 0x08)
		{
			case 0x08: return 0x00 + OtherMeta; // Top   -> Bottom
			case 0x00: return 0x08 + OtherMeta; // Bottom-> Top
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Bits 0x08 and 0x04 (Bits #3 and #4) are flags; lowest two bits are for position. 0x0C = 1100
		NIBBLETYPE OtherMeta = a_Meta & 0x0C;
		// Mirrors by returning appropate metavalue; values are determined by a Metadata table. 0x03 == 0011
		switch (a_Meta & 0x03)
		{
			case 0x02: return 0x03 + OtherMeta; // East  -> West
			case 0x03: return 0x02 + OtherMeta; // West  -> East
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
};




