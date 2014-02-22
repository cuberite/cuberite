#pragma once

#include "BlockHandler.h"
#include "Chunk.h"




class cBlockButtonHandler :
	public cBlockHandler
{
public:
	cBlockButtonHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		// Set p the ON bit to on
		NIBBLETYPE Meta = (a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) | 0x08);

		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect("random.click", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, (Meta & 0x08) ? 0.6f : 0.5f);

		// Queue a button reset (unpress)
		a_ChunkInterface.QueueSetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, (a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x07), m_BlockType == E_BLOCK_STONE_BUTTON ? 20 : 30, m_BlockType, a_WorldInterface);
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
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = BlockFaceToMetaData(a_BlockFace);
		return true;
	}


	virtual const char * GetStepSound(void) override
	{
		return m_BlockType == E_BLOCK_WOODEN_BUTTON ? "step.wood" : "step.stone";
	}


	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_ZM: return 0x4;
			case BLOCK_FACE_ZP: return 0x3;
			case BLOCK_FACE_XM: return 0x2;
			case BLOCK_FACE_XP: return 0x1;
			default:
			{
				ASSERT(!"Unhandled block face!");
				return 0x0; // No idea, give a special meta (button in centre of block)
			}
		}
	}

	inline static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case 0x1: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_ZM;
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

		return (a_RelY > 0) && (g_BlockIsSolid[BlockIsOn]);
	}

	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Holds extraneous metadata values.  Position data uses first 4 bits;   0x07 == 1111
		NIBBLETYPE OtherMeta = a_Meta & (CHAR_MAX - 0x07);
		// Rotates according to a table.
		switch (a_Meta & 0x07)
		{ 
			case 0x01: return 0x04 + OtherMeta;  // East  -> North
			case 0x04: return 0x02 + OtherMeta;  // North -> West
			case 0x02: return 0x03 + OtherMeta;  // West  -> South
			case 0x03: return 0x01 + OtherMeta;  // South -> East
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}

	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Holds extraneous metadata values.  Position data uses first 4 bits;   0x07 == 1111
		NIBBLETYPE OtherMeta = a_Meta & (CHAR_MAX - 0x07);
		// Rotates according to a table.  0x07 == 1111
		switch (a_Meta & 0x07)
		{
			case 0x01: return 0x03 + OtherMeta;  // East  -> South
			case 0x03: return 0x02 + OtherMeta;  // South -> West
			case 0x02: return 0x04 + OtherMeta;  // West  -> North
			case 0x04: return 0x01 + OtherMeta;  // North -> East
		}
		// Not reachable, but to avoid a compiler warning:
		return a_Meta;
	}

	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Holds extraneous metadata values.  Position data uses first 4 bits;   0x07 == 1111
		NIBBLETYPE OtherMeta = a_Meta & (CHAR_MAX - 0x07);
		// Mirrors according to a table.  0x07 == 1111
		switch (a_Meta & 0x07)
		{
			case 0x03: return 0x04 + OtherMeta;  // South -> North
			case 0x04: return 0x03 + OtherMeta;  // North -> South
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}

	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Holds extraneous metadata values.  Position data uses first 4 bits;   0x07 == 1111
		NIBBLETYPE OtherMeta = a_Meta & (CHAR_MAX - 0x07);
		// Mirrors according to a table.  0x07 == 1111
		switch (a_Meta & 0x07)
		{
		case 0x01: return 0x02 + OtherMeta;  // East  -> West
		case 0x02: return 0x01 + OtherMeta;  // West  -> East
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
} ;