
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
		a_BlockY--; // Because we want the block below the fire
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

	inline static void HandeNetherPortal(int X, int Y, int Z, cWorld * a_World) // Brought to you by Aperture Science
	{
		static const struct // Define a base template for checking portal-eligibility
		{
			int xz, y;
		} PortalTemplate[] =
		{
			{-1, 0},
			{-2, 1},
			{-2, 2},
			{-2, 3},
			{-1, 4},
			{ 0, 4},
			{ 1, 1},
			{ 1, 2},
			{ 1, 3},
		} ;

		const int NumberOfDirections = 4; // How many cases we have to check, basically
		const int TemplateCount = ARRAYCOUNT(PortalTemplate);
		int PortalNumber = 0;
		int PortalBlock = 0;

		for (int i = 0; i < NumberOfDirections; i++)
		{
			switch (i)
			{
				case 0:
				{
					for (int x = 0; x < TemplateCount; x++)
					{
						if (a_World->GetBlock(X + PortalTemplate[x].xz, Y + PortalTemplate[x].y, Z) != E_BLOCK_OBSIDIAN)
						{
							goto nexti; // Because gotos are underused but secretly awesome
						}
					}
					PortalNumber = 1; PortalBlock = 1; // No goto break, we have an eligible portal frame!
					break;
				}
				case 1:
				{
					for (int x = 0; x < TemplateCount; x++)
					{
						if (a_World->GetBlock(X - PortalTemplate[x].xz, Y + PortalTemplate[x].y, Z) != E_BLOCK_OBSIDIAN)
						{
							goto nexti;
						}
					}
					PortalNumber = 1; PortalBlock = 2;
					break;
				}
				case 2:
				{
					for (int x = 0; x < TemplateCount; x++)
					{
						if (a_World->GetBlock(X, Y + PortalTemplate[x].y, Z + PortalTemplate[x].xz) != E_BLOCK_OBSIDIAN)
						{
							goto nexti;
						}
					}
					PortalNumber = 2; PortalBlock = 1;
					break;
				}
				case 3:
				{
					for (int x = 0; x < TemplateCount; x++)
					{
						if (a_World->GetBlock(X, Y + PortalTemplate[x].y, Z - PortalTemplate[x].xz) != E_BLOCK_OBSIDIAN)
						{
							goto nexti;
						}
					}
					PortalNumber = 2; PortalBlock = 2;
					break;
				}
			}

		nexti:
			continue;
		}

		if ((PortalNumber != 0) && (PortalBlock != 0))
		{
			SetPortalBlocks(X, Y, Z, PortalNumber, PortalBlock, a_World);
		}
	}

	inline static void SetPortalBlocks(int X, int Y, int Z, int a_PortalType, int a_PortalBlock, cWorld * a_World)
	{
		static const struct
		{
			int xz, y;
		} PortalSetTemplate[] =
		{
			{ 0, 1},
			{ 0, 2},
			{ 0, 3},
			{-1, 1},
			{-1, 2},
			{-1, 3},
		};

		const int TemplateCount = ARRAYCOUNT(PortalSetTemplate);

		if (a_PortalType == 1)
		{
			if (a_PortalBlock == 1)
			{
				for (int i = 0; i < TemplateCount; i++)
				{
					BLOCKTYPE InsideBlock = a_World->GetBlock(X + PortalSetTemplate[i].xz, Y + PortalSetTemplate[i].y, Z);
					if ((InsideBlock != E_BLOCK_AIR) && (InsideBlock != E_BLOCK_FIRE))
					{
						return; // Interior of frame wasn't empty, abort
					}
				}
				for (int i = 0; i < TemplateCount; i++)
				{
					// Meta doesn't do anything on client, but tells server what direction portal block is
					// This is to allow the server to check for invalid positionings
					a_World->SetBlock(X + PortalSetTemplate[i].xz, Y + PortalSetTemplate[i].y, Z, E_BLOCK_NETHER_PORTAL, 1);
				}
			}
			else if (a_PortalBlock == 2)
			{
				for (int i = 0; i < TemplateCount; i++)
				{
					BLOCKTYPE InsideBlock = a_World->GetBlock(X - PortalSetTemplate[i].xz, Y + PortalSetTemplate[i].y, Z);
					if ((InsideBlock != E_BLOCK_AIR) && (InsideBlock != E_BLOCK_FIRE))
					{
						return;
					}
				}
				for (int i = 0; i < TemplateCount; i++)
				{
					a_World->SetBlock(X - PortalSetTemplate[i].xz, Y + PortalSetTemplate[i].y, Z, E_BLOCK_NETHER_PORTAL, 1);
				}
			}
			else { ASSERT(!"Bad nether portal bottom clicked block!"); return; }
		}
		else if (a_PortalType == 2)
		{
			if (a_PortalBlock == 1)
			{
				for (int i = 0; i < TemplateCount; i++)
				{
					BLOCKTYPE InsideBlock = a_World->GetBlock(X, Y + PortalSetTemplate[i].y, Z + PortalSetTemplate[i].xz);
					if ((InsideBlock != E_BLOCK_AIR) && (InsideBlock != E_BLOCK_FIRE))
					{
						return;
					}
				}
				for (int i = 0; i < TemplateCount; i++)
				{
					a_World->SetBlock(X, Y + PortalSetTemplate[i].y, Z + PortalSetTemplate[i].xz, E_BLOCK_NETHER_PORTAL, 2);
				}
			}
			else if (a_PortalBlock == 2)
			{
				for (int i = 0; i < TemplateCount; i++)
				{
					BLOCKTYPE InsideBlock = a_World->GetBlock(X, Y + PortalSetTemplate[i].y, Z - PortalSetTemplate[i].xz);
					if ((InsideBlock != E_BLOCK_AIR) && (InsideBlock != E_BLOCK_FIRE))
					{
						return;
					}
				}
				for (int i = 0; i < TemplateCount; i++)
				{
					a_World->SetBlock(X, Y + PortalSetTemplate[i].y, Z - PortalSetTemplate[i].xz, E_BLOCK_NETHER_PORTAL, 2);
				}
			}
			else { ASSERT(!"Bad nether portal bottom clicked block!"); return; }
		}
		else { ASSERT(!"Bad nether portal orientation!"); return; }
	}
} ;




