
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

	/// Portal boundary and direction variables
	int XZP, XZM, Dir; // For wont of a better name...

	virtual void OnPlaced(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		/*
		PORTAL FINDING ALGORITH
		=======================
		-Get clicked base block
		-Trace upwards to find first obsidian block; aborts if anything other than obsidian or air is encountered.
		 Uses this value as a reference (the 'ceiling')
		-For both directions (if one fails, try the other), BASE (clicked) block:
		 -Go in one direction, only stop if a non obsidian block is encountered (abort) OR a portal border is encountered (FindObsidianCeiling returns -1)
		 -If a border was encountered, go the other direction and repeat above
		 -Write borders to XZP and XZM, write direction portal faces to Dir
		-Loop through boundary variables, and fill with portal blocks based on Dir with meta from Dir		
		*/

		a_BlockY--; // Because we want the block below the fire
		FindAndSetPortalFrame(a_BlockX, a_BlockY, a_BlockZ, a_World); // Brought to you by Aperture Science
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

	/// Traces along YP until it finds an obsidian block, returns Y difference or 0 if no portal, and -1 for border
	/// Takes the X, Y, and Z of the base block; with an optional MaxY for portal border finding
	int FindObsidianCeiling(int X, int Y, int Z, cWorld * a_World, int MaxY = 0)
	{
		if (a_World->GetBlock(X, Y, Z) != E_BLOCK_OBSIDIAN)
		{
			return 0;
		}

		int newY = Y + 1;

		for (newY; newY < cChunkDef::Height; newY++)
		{
			BLOCKTYPE Block = a_World->GetBlock(X, newY, Z);
			if ((Block == E_BLOCK_AIR) || (Block == E_BLOCK_FIRE))
			{
				continue;
			}
			else if (Block == E_BLOCK_OBSIDIAN)
			{
				// We found an obsidian ceiling
				// Make sure MaxY has a value and newY ('ceiling' location) is at one above the base block
				// This is because the frame is a solid obsidian pillar
				if ((MaxY != 0) && (newY == Y + 1))
				{
					return EvaluatePortalBorder(X, newY, Z, MaxY, a_World);
				}
				else
				{
					// Return ceiling Y, whoever called this function will decide if it's part of a portal or not
					return newY;
				}
			}
			else { return 0; }
		}

		return 0;
	}

	/// Evaluates if coords have a valid border on top, based on MaxY
	int EvaluatePortalBorder(int X, int FoundObsidianY, int Z, int MaxY, cWorld * a_World)
	{
		for (int checkBorder = FoundObsidianY + 1; checkBorder <= MaxY - 1; checkBorder++) // FoundObsidianY + 1: FoundObsidianY has already been checked in FindObsidianCeiling; MaxY - 1: portal doesn't need corners
		{
			if (a_World->GetBlock(X, checkBorder, Z) != E_BLOCK_OBSIDIAN)
			{
				// Base obsidian, base + 1 obsidian, base + x NOT obsidian -> not complete portal
				return 0;
			}
		}
		// Everything was obsidian, found a border!
		return -1; // Return -1 for a frame border
	}

	/// Finds entire frame in any direction with the coordinates of a base block and fills hole with nether portal (START HERE)
	void FindAndSetPortalFrame(int X, int Y, int Z, cWorld * a_World)
	{
		int MaxY = FindObsidianCeiling(X, Y, Z, a_World); // Get topmost obsidian block as reference for all other checks
		int X1 = X + 1, Z1 = Z + 1, X2 = X - 1, Z2 = Z - 1; // Duplicate XZ values, add/subtract one as we've checked the original already the line above

		if (MaxY == 0) // Oh noes! Not a portal coordinate :(
		{
			return;
		}

		if (!FindPortalSliceX(X1, X2, Y, Z, MaxY, a_World))
		{
			if (!FindPortalSliceZ(X, Y, Z1, Z2, MaxY, a_World))
			{
				return; // No eligible portal construct, abort abort abort!!
			}
		}

		for (int Height = Y + 1; Height <= MaxY - 1; Height++) // Loop through boundary to set portal blocks
		{
			for (int Width = XZM; Width <= XZP; Width++)
			{
				if (Dir == 1)
				{
					a_World->SetBlock(Width, Height, Z, E_BLOCK_NETHER_PORTAL, Dir);
				}
				else
				{
					a_World->SetBlock(X, Height, Width, E_BLOCK_NETHER_PORTAL, Dir);
				}
			}
		}

		return;
	}

	/// Evaluates if coordinates are a portal going XP/XM; returns true if so, and writes boundaries to variable
	/// Takes coordinates of base block and Y coord of target obsidian ceiling
	bool FindPortalSliceX(int X1, int X2, int Y, int Z, int MaxY, cWorld * a_World)
	{
		Dir = 1; // Set assumed direction (will change if portal turns out to be facing the other direction)
		bool FoundFrameXP = false, FoundFrameXM = false;
		for (X1; ((a_World->GetBlock(X1, Y, Z) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X1, Y + 1, Z) == E_BLOCK_OBSIDIAN)); X1++) // Check XP for obsidian blocks, exempting corners
		{
			int Value = FindObsidianCeiling(X1, Y, Z, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X1, Y + 1, Z, a_World, MaxY); // For corners without obsidian
			if ((Value == -1) || (ValueTwo == -1)) // FindObsidianCeiling returns -1 upon frame-find
			{
				FoundFrameXP = true; // Found a frame border in this direction, proceed in other direction (don't go further)
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY)) // Make sure that there is a valid portal 'slice'
			{
				return false; // Not valid slice, no portal can be formed
			}
		} XZP = X1 - 1; // Set boundary of frame interior, note that for some reason, the loop of X and the loop of Z go to different numbers, hence -1 here and -2 there
		for (X2; ((a_World->GetBlock(X2, Y, Z) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X2, Y + 1, Z) == E_BLOCK_OBSIDIAN)); X2--) // Go the other direction (XM)
		{
			int Value = FindObsidianCeiling(X2, Y, Z, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X2, Y + 1, Z, a_World, MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameXM = true;
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				return false;
			}
		} XZM = X2 + 1; // Set boundary, see previous
		return (FoundFrameXP && FoundFrameXM);
	}

	/// Evaluates if coords are a portal going ZP/ZM; returns true if so, and writes boundaries to variable
	bool FindPortalSliceZ(int X, int Y, int Z1, int Z2, int MaxY, cWorld * a_World)
	{
		Dir = 2;
		bool FoundFrameZP = false, FoundFrameZM = false;
		for (Z1; ((a_World->GetBlock(X, Y, Z1) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X, Y + 1, Z1) == E_BLOCK_OBSIDIAN)); Z1++)
		{
			int Value = FindObsidianCeiling(X, Y, Z1, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X, Y + 1, Z1, a_World, MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameZP = true;
				continue;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				return false;
			}
		} XZP = Z1 - 2;
		for (Z2; ((a_World->GetBlock(X, Y, Z2) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X, Y + 1, Z2) == E_BLOCK_OBSIDIAN)); Z2--)
		{
			int Value = FindObsidianCeiling(X, Y, Z2, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X, Y + 1, Z2, a_World, MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameZM = true;
				continue;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				return false;
			}
		} XZM = Z2 + 2;
		return (FoundFrameZP && FoundFrameZM);
	}
};




