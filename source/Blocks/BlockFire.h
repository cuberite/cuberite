
#pragma once

#include "BlockHandler.h"





class cBlockFireHandler :
	public cBlockHandler
{
public:
	cBlockFireHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void OnPlaced(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		a_BlockY--; // Because we want block below fire
		HandeNetherPortal(a_BlockX, a_BlockY, a_BlockZ, a_World);
	}

	virtual void OnDigging(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		a_World->DigBlock(a_BlockX, a_BlockY, a_BlockZ);
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups from this block
	}

	virtual bool IsClickedThrough(void) override
	{
		return true;
	}
	
	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}

	

	inline static void HandeNetherPortal(int X, int Y, int Z, cWorld * a_World)
	{
		static const struct
		{
			int x, y, z;
		} Portal1Block1[] =
		{
			{-1, 0,  0},
			{-2, 1,  0},
			{-2, 2,  0},
			{-2, 3,  0},
			{-1, 4,  0},
			{ 0, 4,  0},
			{ 1, 1,  0},
			{ 1, 2,  0},
			{ 1, 3,  0},
		} ;

		static const struct
		{
			int x, y, z;
		} Portal1Block2[] =
		{
			{ 1, 0,  0},
			{ 2, 1,  0},
			{ 2, 2,  0},
			{ 2, 3,  0},
			{ 1, 4,  0},
			{ 0, 4,  0},
			{-1, 1,  0},
			{-1, 2,  0},
			{-1, 3,  0},
		} ;

		static const struct
		{
			int x, y, z;
		} Portal2Block1[] =
		{
			{ 0, 0, -1},
			{ 0, 1, -2},
			{ 0, 2, -2},
			{ 0, 3, -2},
			{ 0, 4, -1},
			{ 0, 4,  0},
			{ 0, 1,  1},
			{ 0, 2,  1},
			{ 0, 3,  1},
		} ;

		static const struct
		{
			int x, y, z;
		} Portal2Block2[] =
		{
			{ 0, 0,  1},
			{ 0, 1,  2},
			{ 0, 2,  2},
			{ 0, 3,  2},
			{ 0, 4,  1},
			{ 0, 4,  0},
			{ 0, 1, -1},
			{ 0, 2, -1},
			{ 0, 3, -1},
		} ;

		bool AllObsidian = false;
		int PortalNumber = 0;
		int PortalBlock = 0;

		bool Portal1Block1Success = true;
		for (int i = 0; i < ARRAYCOUNT(Portal1Block1); i++)
		{
			if (a_World->GetBlock(X + Portal1Block1[i].x, Y + Portal1Block1[i].y, Z + Portal1Block1[i].z) != E_BLOCK_OBSIDIAN)
			{
				Portal1Block1Success = false; // Wasn't complete frame, abort
				break;
			}
		}
		if (Portal1Block1Success)
		{
			AllObsidian = true;
			PortalNumber = 1;
			PortalBlock = 1;
		}

		bool Portal1Block2Success = true;
		for (int i = 0; i < ARRAYCOUNT(Portal1Block2); i++)
		{
			if (a_World->GetBlock(X + Portal1Block2[i].x, Y + Portal1Block2[i].y, Z + Portal1Block2[i].z) != E_BLOCK_OBSIDIAN)
			{
				Portal1Block2Success = false;
				break;
			}
		}
		if (Portal1Block2Success)
		{
			AllObsidian = true;
			PortalNumber = 1;
			PortalBlock = 2;
		}

		bool Portal2Block1Success = true;
		for (int i = 0; i < ARRAYCOUNT(Portal2Block1); i++)
		{
			if (a_World->GetBlock(X + Portal2Block1[i].x, Y + Portal2Block1[i].y, Z + Portal2Block1[i].z) != E_BLOCK_OBSIDIAN)
			{
				Portal2Block1Success = false;
				break;
			}
		}
		if (Portal2Block1Success)
		{
			AllObsidian = true;
			PortalNumber = 2;
			PortalBlock = 1;
		}

		bool Portal2Block2Success = true;
		for (int i = 0; i < ARRAYCOUNT(Portal2Block2); i++)
		{
			if (a_World->GetBlock(X + Portal2Block2[i].x, Y + Portal2Block2[i].y, Z + Portal2Block2[i].z) != E_BLOCK_OBSIDIAN)
			{
				Portal2Block2Success = false;
				break;
			}
		}
		if (Portal2Block2Success)
		{
			AllObsidian = true;
			PortalNumber = 2;
			PortalBlock = 2;
		}

		if (AllObsidian)
		{
			SetPortalBlocks(X, Y, Z, PortalNumber, PortalBlock, a_World);
		}
	}


	inline static void SetPortalBlocks(int X, int Y, int Z, int a_PortalType, int a_PortalBlock, cWorld * a_World)
	{
		if (a_PortalType == 1)
		{
			if (a_PortalBlock == 1)
			{
				static const struct
				{
					int x, y, z;
				} Portal1Block1Set[] =
				{
					{ 0, 1,  0},
					{ 0, 2,  0},
					{ 0, 3,  0},
					{-1, 1,  0},
					{-1, 2,  0},
					{-1, 3,  0},
				} ;

				for (int i = 0; i < ARRAYCOUNT(Portal1Block1Set); i++)
				{
					BLOCKTYPE InsideBlock = a_World->GetBlock(X + Portal1Block1Set[i].x, Y + Portal1Block1Set[i].y, Z + Portal1Block1Set[i].z);
					if ((InsideBlock != E_BLOCK_AIR) && (InsideBlock != E_BLOCK_FIRE))
					{
						return; // Interior of frame wasn't empty, abort
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal1Block1Set); i++)
				{
					// Meta doesn't do anything on client, but tells server what direction portal block is
					// This is to allow the server to check for invalid positionings
					a_World->SetBlock(X + Portal1Block1Set[i].x, Y + Portal1Block1Set[i].y, Z + Portal1Block1Set[i].z, E_BLOCK_NETHER_PORTAL, 1);
				}
			}
			else if (a_PortalBlock == 2)
			{
				static const struct
				{
					int x, y, z;
				} Portal1Block2Set[] =
				{
					{ 0, 1,  0},
					{ 0, 2,  0},
					{ 0, 3,  0},
					{ 1, 1,  0},
					{ 1, 2,  0},
					{ 1, 3,  0},
				} ;

				for (int i = 0; i < ARRAYCOUNT(Portal1Block2Set); i++)
				{
					BLOCKTYPE InsideBlock = a_World->GetBlock(X + Portal1Block2Set[i].x, Y + Portal1Block2Set[i].y, Z + Portal1Block2Set[i].z);
					if ((InsideBlock != E_BLOCK_AIR) && (InsideBlock != E_BLOCK_FIRE))
					{
						return;
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal1Block2Set); i++)
				{
					a_World->SetBlock(X + Portal1Block2Set[i].x, Y + Portal1Block2Set[i].y, Z + Portal1Block2Set[i].z, E_BLOCK_NETHER_PORTAL, 1);
				}
			}
			else { ASSERT(!"Bad nether portal bottom clicked block!"); return; }
		}
		else if (a_PortalType == 2)
		{
			if (a_PortalBlock == 1)
			{
				static const struct
				{
					int x, y, z;
				} Portal2Block1Set[] =
				{
					{ 0, 1,  0},
					{ 0, 2,  0},
					{ 0, 3,  0},
					{ 0, 1, -1},
					{ 0, 2, -1},
					{ 0, 3, -1},
				} ;

				for (int i = 0; i < ARRAYCOUNT(Portal2Block1Set); i++)
				{
					BLOCKTYPE InsideType = a_World->GetBlock(X + Portal2Block1Set[i].x, Y + Portal2Block1Set[i].y, Z + Portal2Block1Set[i].z);
					if ((InsideType != E_BLOCK_AIR) && (InsideType != E_BLOCK_FIRE))
					{
						return;
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal2Block1Set); i++)
				{
					a_World->SetBlock(X + Portal2Block1Set[i].x, Y + Portal2Block1Set[i].y, Z + Portal2Block1Set[i].z, E_BLOCK_NETHER_PORTAL, 2);
				}
			}
			else if (a_PortalBlock == 2)
			{
				static const struct
				{
					int x, y, z;
				} Portal2Block2Set[] =
				{
					{ 0, 1,  0},
					{ 0, 2,  0},
					{ 0, 3,  0},
					{ 0, 1,  1},
					{ 0, 2,  1},
					{ 0, 3,  1},
				} ;

				for (int i = 0; i < ARRAYCOUNT(Portal2Block2Set); i++)
				{
					BLOCKTYPE InsideBlock = a_World->GetBlock(X + Portal2Block2Set[i].x, Y + Portal2Block2Set[i].y, Z + Portal2Block2Set[i].z);
					if ((InsideBlock != E_BLOCK_AIR) && (InsideBlock != E_BLOCK_FIRE))
					{
						return;
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal2Block2Set); i++)
				{
					a_World->SetBlock(X + Portal2Block2Set[i].x, Y + Portal2Block2Set[i].y, Z + Portal2Block2Set[i].z, E_BLOCK_NETHER_PORTAL, 2);
				}
			}
			else { ASSERT(!"Bad nether portal bottom clicked block!"); return; }
		}
		else { ASSERT(!"Bad nether portal orientation!"); return; }
	}
} ;




