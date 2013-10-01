
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Entities/TNTEntity.h"





class cItemLighterHandler :
	public cItemHandler
{
public:
	cItemLighterHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (a_BlockFace < 0)
		{
			return false;
		}
		
		a_Player->UseEquippedItem();		

		switch (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ))
		{
			case E_BLOCK_TNT:
			{
				// Activate the TNT:
				a_World->BroadcastSoundEffect("random.fuse", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.6f);
				a_World->SpawnPrimedTNT(a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5, 4);  // 4 seconds to boom
				a_World->SetBlock(a_BlockX,a_BlockY,a_BlockZ, E_BLOCK_AIR, 0);
				break;
			}
			case E_BLOCK_OBSIDIAN:
			{
				if (!HandeNetherPortal(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_World))
				{
					// Couldn't create a nether portal, do a normal firelight
					AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
					if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_AIR)
					{
						a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FIRE, 0);
					}
				}
				break;
			}
			default:
			{
				// Light a fire next to/on top of the block if air:
				AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
				if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_AIR)
				{
					a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FIRE, 0);
					break;
				}
			}
		}
		return false;
	}

	inline static bool HandeNetherPortal(int X, int Y, int Z, int Face, cWorld * a_World)
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
			return SetPortalBlocks(X, Y, Z, PortalNumber, PortalBlock, a_World);
		}
		else { return false; }
	}


	inline static bool SetPortalBlocks(int X, int Y, int Z, int a_PortalType, int a_PortalBlock, cWorld * a_World)
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
					if (a_World->GetBlock(X + Portal1Block1Set[i].x, Y + Portal1Block1Set[i].y, Z + Portal1Block1Set[i].z) != E_BLOCK_AIR)
					{
						return false; // Interior of frame wasn't empty, abort
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal1Block1Set); i++)
				{
					a_World->SetBlock(X + Portal1Block1Set[i].x, Y + Portal1Block1Set[i].y, Z + Portal1Block1Set[i].z, E_BLOCK_NETHER_PORTAL, 0); // Meta is calculated on client
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
					if (a_World->GetBlock(X + Portal1Block2Set[i].x, Y + Portal1Block2Set[i].y, Z + Portal1Block2Set[i].z) != E_BLOCK_AIR)
					{
						return false;
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal1Block2Set); i++)
				{
					a_World->SetBlock(X + Portal1Block2Set[i].x, Y + Portal1Block2Set[i].y, Z + Portal1Block2Set[i].z, E_BLOCK_NETHER_PORTAL, 0);
				}
			}
			else { ASSERT(!"Bad nether portal bottom clicked block!"); return false; }
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
					if (a_World->GetBlock(X + Portal2Block1Set[i].x, Y + Portal2Block1Set[i].y, Z + Portal2Block1Set[i].z) != E_BLOCK_AIR)
					{
						return false;
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal2Block1Set); i++)
				{
					a_World->SetBlock(X + Portal2Block1Set[i].x, Y + Portal2Block1Set[i].y, Z + Portal2Block1Set[i].z, E_BLOCK_NETHER_PORTAL, 0);
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
					if (a_World->GetBlock(X + Portal2Block2Set[i].x, Y + Portal2Block2Set[i].y, Z + Portal2Block2Set[i].z) != E_BLOCK_AIR)
					{
						return false;
					}
				}
				for (int i = 0; i < ARRAYCOUNT(Portal2Block2Set); i++)
				{
					a_World->SetBlock(X + Portal2Block2Set[i].x, Y + Portal2Block2Set[i].y, Z + Portal2Block2Set[i].z, E_BLOCK_NETHER_PORTAL, 0);
				}
			}
			else { ASSERT(!"Bad nether portal bottom clicked block!"); return false; }
		}
		else { ASSERT(!"Bad nether portal orientation!"); return false; }
		return true; // Will default to this if everything was successful
	}
} ;




