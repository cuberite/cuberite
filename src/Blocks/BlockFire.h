
#pragma once

#include "BlockHandler.h"





class cBlockFireHandler :
	public cBlockHandler
{
public:
	cBlockFireHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType),
		m_XZP(0), m_XZM(0), m_Dir(0)
	{
	}

	/** Portal boundary and direction variables */
	// 2014_03_30 _X: What are these used for? Why do we need extra variables?
	int m_XZP, m_XZM;
	NIBBLETYPE m_Dir;

	virtual void OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		/*
		PORTAL FINDING ALGORITH
		=======================
		- Get clicked base block
		- Trace upwards to find first obsidian block; aborts if anything other than obsidian or air is encountered.
			Uses this value as a reference (the 'ceiling')
		- For both directions (if one fails, try the other), BASE (clicked) block:
			- Go in one direction, only stop if a non obsidian block is encountered (abort) OR a portal border is encountered (FindObsidianCeiling returns -1)
			- If a border was encountered, go the other direction and repeat above
			- Write borders to XZP and XZM, write direction portal faces to Dir
		- Loop through boundary variables, and fill with portal blocks based on Dir with meta from Dir
		*/

		// a_BlockY - 1: Because we want the block below the fire
		FindAndSetPortalFrame(a_BlockX, a_BlockY - 1, a_BlockZ, a_ChunkInterface, a_WorldInterface);
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups from this block
	}

	virtual bool IsClickedThrough(void) override
	{
		return true;
	}

	/** Traces along YP until it finds an obsidian block, returns Y difference or 0 if no portal, and -1 for border
	Takes the X, Y, and Z of the base block; with an optional MaxY for portal border finding */
	int FindObsidianCeiling(int a_X, int a_Y, int a_Z, cChunkInterface & a_ChunkInterface, int a_MaxY = 0)
	{
		if (a_ChunkInterface.GetBlock({a_X, a_Y, a_Z}) != E_BLOCK_OBSIDIAN)
		{
			return 0;
		}

		for (int newY = a_Y + 1; newY < cChunkDef::Height; newY++)
		{
			BLOCKTYPE Block = a_ChunkInterface.GetBlock({a_X, newY, a_Z});
			if ((Block == E_BLOCK_AIR) || (Block == E_BLOCK_FIRE))
			{
				continue;
			}
			else if (Block == E_BLOCK_OBSIDIAN)
			{
				// We found an obsidian ceiling
				// Make sure MaxY has a value and newY ('ceiling' location) is at one above the base block
				// This is because the frame is a solid obsidian pillar
				if ((a_MaxY != 0) && (newY == a_Y + 1))
				{
					return EvaluatePortalBorder(a_X, newY, a_Z, a_MaxY, a_ChunkInterface) ? -1 /* -1 = found a frame */ : 0;
				}
				else
				{
					// Return ceiling Y, whoever called this function will decide if it's part of a portal or not
					return newY;
				}
			}
		}

		return 0;
	}

	/** Evaluates if coords have a valid border on top, based on MaxY */
	bool EvaluatePortalBorder(int a_X, int a_FoundObsidianY, int a_Z, int a_MaxY, cChunkInterface & a_ChunkInterface)
	{
		for (int checkBorder = a_FoundObsidianY + 1; checkBorder <= a_MaxY - 1; checkBorder++)  // FoundObsidianY + 1: FoundObsidianY has already been checked in FindObsidianCeiling; MaxY - 1: portal doesn't need corners
		{
			if (a_ChunkInterface.GetBlock({a_X, checkBorder, a_Z}) != E_BLOCK_OBSIDIAN)
			{
				// Base obsidian, base + 1 obsidian, base + x NOT obsidian -> not complete portal
				return false;
			}
		}
		// Everything was obsidian, found a border!
		return true;
	}

	/** Finds entire frame in any direction with the coordinates of a base block and fills hole with nether portal (START HERE) */
	void FindAndSetPortalFrame(int a_X, int a_Y, int a_Z, cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface)
	{
		int MaxY = FindObsidianCeiling(a_X, a_Y, a_Z, a_ChunkInterface);  // Get topmost obsidian block as reference for all other checks
		int X1 = a_X + 1, Z1 = a_Z + 1, X2 = a_X - 1, Z2 = a_Z - 1;  // Duplicate XZ values, add / subtract one as we've checked the original already the line above

		if (MaxY == 0)  // Oh noes! Not a portal coordinate :(
		{
			return;
		}

		if (!FindPortalSliceX(X1, X2, a_Y, a_Z, MaxY, a_ChunkInterface))
		{
			if (!FindPortalSliceZ(a_X, a_Y, Z1, Z2, MaxY, a_ChunkInterface))
			{
				return;  // No eligible portal construct, abort abort abort!!
			}
		}

		int PortalHeight = MaxY - a_Y - 1;
		int PortalWidth = XZP - XZM + 1;
		if ((PortalHeight < a_WorldInterface.GetMinNetherPortalHeight()) || (PortalHeight > a_WorldInterface.GetMaxNetherPortalHeight()))
		{
			// The portal isn't high enough, or is too high
			return;
		}

		if ((PortalWidth < a_WorldInterface.GetMinNetherPortalWidth()) || (PortalWidth > a_WorldInterface.GetMaxNetherPortalWidth()))
		{
			// The portal isn't wide enough, or is too wide
			return;
		}

		for (int Height = a_Y + 1; Height <= MaxY - 1; Height++)  // Loop through boundary to set portal blocks
		{
			for (int Width = XZM; Width <= XZP; Width++)
			{
				if (Dir == 1)
				{
					a_ChunkInterface.SetBlock(Width, Height, a_Z, E_BLOCK_NETHER_PORTAL, Dir);
				}
				else
				{
					a_ChunkInterface.SetBlock(a_X, Height, Width, E_BLOCK_NETHER_PORTAL, Dir);
				}
			}
		}

		return;
	}

	/** Evaluates if coordinates are a portal going XP / XM; returns true if so, and writes boundaries to variable
	Takes coordinates of base block and Y coord of target obsidian ceiling */
	bool FindPortalSliceX(int a_X1, int a_X2, int a_Y, int a_Z, int a_MaxY, cChunkInterface & a_ChunkInterface)
	{
		Dir = 1;  // Set assumed direction (will change if portal turns out to be facing the other direction)
		bool FoundFrameXP = false, FoundFrameXM = false;
		for (; ((a_ChunkInterface.GetBlock({a_X1, a_Y, a_Z}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({a_X1, a_Y + 1, a_Z}) == E_BLOCK_OBSIDIAN)); a_X1++)  // Check XP for obsidian blocks, exempting corners
		{
			int Value = FindObsidianCeiling(a_X1, a_Y, a_Z, a_ChunkInterface, a_MaxY);
			int ValueTwo = FindObsidianCeiling(a_X1, a_Y + 1, a_Z, a_ChunkInterface, a_MaxY);  // For corners without obsidian
			if ((Value == -1) || (ValueTwo == -1))  // FindObsidianCeiling returns -1 upon frame-find
			{
				FoundFrameXP = true;  // Found a frame border in this direction, proceed in other direction (don't go further)
				break;
			}
			else if ((Value != a_MaxY) && (ValueTwo != a_MaxY))  // Make sure that there is a valid portal 'slice'
			{
				return false;  // Not valid slice, no portal can be formed
			}
		}
		XZP = a_X1 - 1;  // Set boundary of frame interior
		for (; ((a_ChunkInterface.GetBlock({a_X2, a_Y, a_Z}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({a_X2, a_Y + 1, a_Z}) == E_BLOCK_OBSIDIAN)); a_X2--)  // Go the other direction (XM)
		{
			int Value = FindObsidianCeiling(a_X2, a_Y, a_Z, a_ChunkInterface, a_MaxY);
			int ValueTwo = FindObsidianCeiling(a_X2, a_Y + 1, a_Z, a_ChunkInterface, a_MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameXM = true;
				break;
			}
			else if ((Value != a_MaxY) && (ValueTwo != a_MaxY))
			{
				return false;
			}
		}
		XZM = a_X2 + 1;  // Set boundary, see previous

		return (FoundFrameXP && FoundFrameXM);
	}

	/** Evaluates if coords are a portal going ZP / ZM; returns true if so, and writes boundaries to variable */
	bool FindPortalSliceZ(int a_X, int a_Y, int a_Z1, int a_Z2, int a_MaxY, cChunkInterface & a_ChunkInterface)
	{
		Dir = 2;
		bool FoundFrameZP = false, FoundFrameZM = false;
		for (; ((a_ChunkInterface.GetBlock({a_X, a_Y, a_Z1}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({a_X, a_Y + 1, a_Z1}) == E_BLOCK_OBSIDIAN)); a_Z1++)
		{
			int Value = FindObsidianCeiling(a_X, a_Y, a_Z1, a_ChunkInterface, a_MaxY);
			int ValueTwo = FindObsidianCeiling(a_X, a_Y + 1, a_Z1, a_ChunkInterface, a_MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameZP = true;
				break;
			}
			else if ((Value != a_MaxY) && (ValueTwo != a_MaxY))
			{
				return false;
			}
		}
		XZP = a_Z1 - 1;
		for (; ((a_ChunkInterface.GetBlock({a_X, a_Y, a_Z2}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({a_X, a_Y + 1, a_Z2}) == E_BLOCK_OBSIDIAN)); a_Z2--)
		{
			int Value = FindObsidianCeiling(a_X, a_Y, a_Z2, a_ChunkInterface, a_MaxY);
			int ValueTwo = FindObsidianCeiling(a_X, a_Y + 1, a_Z2, a_ChunkInterface, a_MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameZM = true;
				break;
			}
			else if ((Value != a_MaxY) && (ValueTwo != a_MaxY))
			{
				return false;
			}
		}
		XZM = a_Z2 + 1;

		return (FoundFrameZP && FoundFrameZM);
	}

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		return true;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 15;
	}
};




