
#pragma once

#include "BlockHandler.h"





class cBlockFireHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	struct Scratch
	{
		/** Portal boundary and direction variables */
		int XZP = 0, XZM = 0;
		NIBBLETYPE Dir = 0;
	};





	virtual void OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) const override
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

		Scratch Scratch;

		// a_BlockY - 1: Because we want the block below the fire
		FindAndSetPortalFrame(a_BlockPos.x, a_BlockPos.y - 1, a_BlockPos.z, a_ChunkInterface, a_WorldInterface, Scratch);
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// No pickups from this block
		return {};
	}





	virtual bool IsClickedThrough(void) const override
	{
		return true;
	}

	/** Traces along YP until it finds an obsidian block, returns Y difference or 0 if no portal, and -1 for border
	Takes the X, Y, and Z of the base block; with an optional MaxY for portal border finding */
	static int FindObsidianCeiling(int X, int Y, int Z, cChunkInterface & a_ChunkInterface, int MaxY = 0)
	{
		if (a_ChunkInterface.GetBlock({X, Y, Z}) != E_BLOCK_OBSIDIAN)
		{
			return 0;
		}

		for (int newY = Y + 1; newY < cChunkDef::Height; newY++)
		{
			BLOCKTYPE Block = a_ChunkInterface.GetBlock({X, newY, Z});
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
					return EvaluatePortalBorder(X, newY, Z, MaxY, a_ChunkInterface) ? -1 /* -1 = found a frame */ : 0;
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
	static bool EvaluatePortalBorder(int X, int FoundObsidianY, int Z, int MaxY, cChunkInterface & a_ChunkInterface)
	{
		for (int checkBorder = FoundObsidianY + 1; checkBorder <= MaxY - 1; checkBorder++)  // FoundObsidianY + 1: FoundObsidianY has already been checked in FindObsidianCeiling; MaxY - 1: portal doesn't need corners
		{
			if (a_ChunkInterface.GetBlock({X, checkBorder, Z}) != E_BLOCK_OBSIDIAN)
			{
				// Base obsidian, base + 1 obsidian, base + x NOT obsidian -> not complete portal
				return false;
			}
		}
		// Everything was obsidian, found a border!
		return true;
	}





	/** Finds entire frame in any direction with the coordinates of a base block and fills hole with nether portal (START HERE) */
	static void FindAndSetPortalFrame(int X, int Y, int Z, cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Scratch & a_Scratch)
	{
		int MaxY = FindObsidianCeiling(X, Y, Z, a_ChunkInterface);  // Get topmost obsidian block as reference for all other checks
		int X1 = X + 1, Z1 = Z + 1, X2 = X - 1, Z2 = Z - 1;  // Duplicate XZ values, add / subtract one as we've checked the original already the line above

		if (MaxY == 0)  // Oh noes! Not a portal coordinate :(
		{
			return;
		}

		if (!FindPortalSliceX(X1, X2, Y, Z, MaxY, a_ChunkInterface, a_Scratch))
		{
			if (!FindPortalSliceZ(X, Y, Z1, Z2, MaxY, a_ChunkInterface, a_Scratch))
			{
				return;  // No eligible portal construct, abort abort abort!!
			}
		}

		int PortalHeight = MaxY - Y - 1;
		int PortalWidth = a_Scratch.XZP - a_Scratch.XZM + 1;
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

		for (int Height = Y + 1; Height <= MaxY - 1; Height++)  // Loop through boundary to set portal blocks
		{
			for (int Width = a_Scratch.XZM; Width <= a_Scratch.XZP; Width++)
			{
				if (a_Scratch.Dir == 1)
				{
					a_ChunkInterface.SetBlock(Width, Height, Z, E_BLOCK_NETHER_PORTAL, a_Scratch.Dir);
				}
				else
				{
					a_ChunkInterface.SetBlock(X, Height, Width, E_BLOCK_NETHER_PORTAL, a_Scratch.Dir);
				}
			}
		}
	}

	/** Evaluates if coordinates are a portal going XP / XM; returns true if so, and writes boundaries to variable
	Takes coordinates of base block and Y coord of target obsidian ceiling */
	static bool FindPortalSliceX(int X1, int X2, int Y, int Z, int MaxY, cChunkInterface & a_ChunkInterface, Scratch & a_Scratch)
	{
		a_Scratch.Dir = 1;  // Set assumed direction (will change if portal turns out to be facing the other direction)
		bool FoundFrameXP = false, FoundFrameXM = false;
		for (; ((a_ChunkInterface.GetBlock({X1, Y, Z}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({X1, Y + 1, Z}) == E_BLOCK_OBSIDIAN)); X1++)  // Check XP for obsidian blocks, exempting corners
		{
			int Value = FindObsidianCeiling(X1, Y, Z, a_ChunkInterface, MaxY);
			int ValueTwo = FindObsidianCeiling(X1, Y + 1, Z, a_ChunkInterface, MaxY);  // For corners without obsidian
			if ((Value == -1) || (ValueTwo == -1))  // FindObsidianCeiling returns -1 upon frame-find
			{
				FoundFrameXP = true;  // Found a frame border in this direction, proceed in other direction (don't go further)
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))  // Make sure that there is a valid portal 'slice'
			{
				return false;  // Not valid slice, no portal can be formed
			}
		}
		a_Scratch.XZP = X1 - 1;  // Set boundary of frame interior
		for (; ((a_ChunkInterface.GetBlock({X2, Y, Z}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({X2, Y + 1, Z}) == E_BLOCK_OBSIDIAN)); X2--)  // Go the other direction (XM)
		{
			int Value = FindObsidianCeiling(X2, Y, Z, a_ChunkInterface, MaxY);
			int ValueTwo = FindObsidianCeiling(X2, Y + 1, Z, a_ChunkInterface, MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameXM = true;
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				return false;
			}
		}
		a_Scratch.XZM = X2 + 1;  // Set boundary, see previous

		return (FoundFrameXP && FoundFrameXM);
	}

	/** Evaluates if coords are a portal going ZP / ZM; returns true if so, and writes boundaries to variable */
	static bool FindPortalSliceZ(int X, int Y, int Z1, int Z2, int MaxY, cChunkInterface & a_ChunkInterface, Scratch & a_Scratch)
	{
		a_Scratch.Dir = 2;
		bool FoundFrameZP = false, FoundFrameZM = false;
		for (; ((a_ChunkInterface.GetBlock({X, Y, Z1}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({X, Y + 1, Z1}) == E_BLOCK_OBSIDIAN)); Z1++)
		{
			int Value = FindObsidianCeiling(X, Y, Z1, a_ChunkInterface, MaxY);
			int ValueTwo = FindObsidianCeiling(X, Y + 1, Z1, a_ChunkInterface, MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameZP = true;
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				return false;
			}
		}
		a_Scratch.XZP = Z1 - 1;
		for (; ((a_ChunkInterface.GetBlock({X, Y, Z2}) == E_BLOCK_OBSIDIAN) || (a_ChunkInterface.GetBlock({X, Y + 1, Z2}) == E_BLOCK_OBSIDIAN)); Z2--)
		{
			int Value = FindObsidianCeiling(X, Y, Z2, a_ChunkInterface, MaxY);
			int ValueTwo = FindObsidianCeiling(X, Y + 1, Z2, a_ChunkInterface, MaxY);
			if ((Value == -1) || (ValueTwo == -1))
			{
				FoundFrameZM = true;
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				return false;
			}
		}
		a_Scratch.XZM = Z2 + 1;

		return (FoundFrameZP && FoundFrameZM);
	}

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) const override
	{
		return true;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 15;
	}
};




