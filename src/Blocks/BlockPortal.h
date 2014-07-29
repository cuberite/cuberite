
#pragma once

#include "BlockHandler.h"
#include "../Mobs/Monster.h"





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
		return;  // No pickups
	}

	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		cFastRandom Random;
		if (Random.NextInt(2000) != 0)
		{
			return;
		}

		int PosX = a_Chunk.GetPosX() * 16 + a_RelX;
		int PosZ = a_Chunk.GetPosZ() * 16 + a_RelZ;

		a_WorldInterface.SpawnMob(PosX, a_RelY, PosZ, cMonster::mtZombiePigman);
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if ((a_RelY - 1 < 0) || (a_RelY + 1 > cChunkDef::Height))
		{
			return false;  // In case someone places a portal with meta 1 or 2 at boundaries, and server tries to get invalid coords at Y - 1 or Y + 1
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
					{ 0, -1,  0},
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
					{ 0, -1,  0},
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
} ;




