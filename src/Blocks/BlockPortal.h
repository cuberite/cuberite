
#pragma once

#include "BlockHandler.h"





class cBlockPortalHandler :
	public cBlockHandler
{
public:
	cBlockPortalHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// We set to zero so MCS doesn't stop you from building weird portals like vanilla does
		// CanBeAt doesn't do anything if meta is zero
		// We set to zero because the client sends meta = 2 to the server (it calculates rotation itself)

		a_BlockType = m_BlockType;
		a_BlockMeta = 0;
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		return; // No pickups
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if ((a_RelY - 1 < 0) || (a_RelY + 1 > cChunkDef::Height))
		{
			return false; // In case someone places a portal with meta 1 or 2 at boundaries, and server tries to get invalid coords at Y - 1 or Y + 1
		}

		switch (a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ))
		{
			case 0x1:
			{
				static const struct
				{
					int x, y, z;
				} PortalCheck[] =
				{
					{ 0, 1,  0},
					{ 0,-1,  0},
					{ 1, 0,  0},
					{-1, 0,  0},
				} ;

				for (size_t i = 0; i < ARRAYCOUNT(PortalCheck); i++)
				{
					BLOCKTYPE Block;
					a_Chunk.UnboundedRelGetBlockType(a_RelX + PortalCheck[i].x, a_RelY + PortalCheck[i].y, a_RelZ + PortalCheck[i].z, Block);

					if ((Block != E_BLOCK_NETHER_PORTAL) && (Block != E_BLOCK_OBSIDIAN))
					{
						return false;
					}
				}
				break;
			}
			case 0x2:
			{
				static const struct
				{
					int x, y, z;
				} PortalCheck[] =
				{
					{ 0, 1,  0},
					{ 0,-1,  0},
					{ 0, 0, -1},
					{ 0, 0,  1},
				} ;

				for (size_t i = 0; i < ARRAYCOUNT(PortalCheck); i++)
				{
					BLOCKTYPE Block;
					a_Chunk.UnboundedRelGetBlockType(a_RelX + PortalCheck[i].x, a_RelY + PortalCheck[i].y, a_RelZ + PortalCheck[i].z, Block);

					if ((Block != E_BLOCK_NETHER_PORTAL) && (Block != E_BLOCK_OBSIDIAN))
					{
						return false;
					}
				}
				break;
			}
		}
		return true;
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// 0x08 bit is a flag (Eye of ender inserted or not?)
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Rotates according to table; 0x07 == 0111.
		switch (a_Meta & 0x07)
		{
			case 0x00: return 0x03 + OtherMeta;  // South -> East
			case 0x03: return 0x02 + OtherMeta;  // East  -> North
			case 0x02: return 0x01 + OtherMeta;  // North -> West
			case 0x01: return 0x00 + OtherMeta;  // West  -> South
		}
		// To avoid a compiler warning;
		return a_Meta;
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// 0x08 bit is a flag (Eye of ender inserted or not?)
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Rotates according to table; 0x07 == 0111.
		switch (a_Meta & 0x07)
		{
			case 0x00: return 0x01 + OtherMeta;  // South -> West
			case 0x01: return 0x02 + OtherMeta;  // West  -> North
			case 0x02: return 0x03 + OtherMeta;  // North -> East
			case 0x03: return 0x00 + OtherMeta;  // East  -> South
		}
		// To avoid a compiler warning;
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// 0x08 bit is a flag (Eye of ender inserted or not?)
		NIBBLETYPE OtherMeta = a_Meta & 0x08;
		// Rotates according to table; 0x07 == 0111.
		switch (a_Meta & 0x07)
		{
			case 0x00: return 0x02 + OtherMeta;  // South -> North
			case 0x02: return 0x00 + OtherMeta;  // North -> South
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




