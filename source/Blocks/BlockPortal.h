
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
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
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


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ) == 0x1)
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

			for (int i = 0; i < ARRAYCOUNT(PortalCheck); i++)
			{
				BLOCKTYPE Block;
				a_Chunk.UnboundedRelGetBlockType(a_RelX + PortalCheck[i].x, a_RelY + PortalCheck[i].y, a_RelZ + PortalCheck[i].z, Block);

				if ((Block != E_BLOCK_NETHER_PORTAL) && (Block != E_BLOCK_OBSIDIAN))
				{
					return false;
				}
			}
		}
		else if (a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ) == 0x2)
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

			for (int i = 0; i < ARRAYCOUNT(PortalCheck); i++)
			{
				BLOCKTYPE Block;
				a_Chunk.UnboundedRelGetBlockType(a_RelX + PortalCheck[i].x, a_RelY + PortalCheck[i].y, a_RelZ + PortalCheck[i].z, Block);

				if ((Block != E_BLOCK_NETHER_PORTAL) && (Block != E_BLOCK_OBSIDIAN))
				{
					return false;
				}
			}
		}
		return true;
	}
} ;




