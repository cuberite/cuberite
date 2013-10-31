
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

	struct sAirBlocksList
	{
		Vector3i a_BlockPos;
		NIBBLETYPE Direction;
	};

	typedef std::vector <sAirBlocksList> AirBlocks;
	AirBlocks m_AirBlocks;

	virtual void OnPlaced(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		a_BlockY--; // Because we want the block below the fire
		FindAndSetPortalFrame(a_BlockX, a_BlockY, a_BlockZ, a_World); // Brought to you by the Aperture Science Handheld Portal Device Testing Programme
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

	int FindObsidianCeiling(int X, int Y, int Z, NIBBLETYPE a_Dir, cWorld * a_World, int MaxY = NULL)
	{
		if (a_World->GetBlock(X, Y, Z) != E_BLOCK_OBSIDIAN)
		{
			return NULL;
		}

		int newY = Y + 1;

		for (newY; newY < cChunkDef::Height; newY++)
		{
			BLOCKTYPE Block = a_World->GetBlock(X, newY, Z);
			if ((Block == E_BLOCK_AIR) || (Block == E_BLOCK_FIRE))
			{
				if (a_Dir != NULL)
				{
					sAirBlocksList RC;
					RC.a_BlockPos = Vector3i(X, newY, Z);
					RC.Direction = a_Dir;
					m_AirBlocks.push_back(RC);
				}
			}
			else if (Block == E_BLOCK_OBSIDIAN)
			{
				// We found an obsidian ceiling
				// Make sure MaxY has a value and newY ('ceiling' location) is at one above the base block
				// This is because the frame is a solid obsidian pillar
				if ((MaxY != NULL) && (newY == Y + 1))
				{
					for (int checkBorder = newY + 1; checkBorder <= MaxY - 1; checkBorder++) // newY + 1: newY has already been checked; MaxY - 1: portal doesn't need corners
					{
						if (a_World->GetBlock(X, checkBorder, Z) != E_BLOCK_OBSIDIAN)
						{
							// Base obsidian, base + 1 obsidian, base + x NOT obsidian -> not complete portal
							return NULL;
						}
					}
					// Everything was obsidian, found a border!
					return NULL - 1; // What do you get if you take something away from nothing? Negative one, apparently :P
				}
				else
				{
					// Return ceiling Y, whoever called this function will decide if it's part of a portal or not
					return newY;
				}
			}
			else { return NULL; }
		}

		return NULL;
	}

	void FindAndSetPortalFrame(int X, int Y, int Z, cWorld * a_World)
	{
		int MaxY = FindObsidianCeiling(X, Y, Z, NULL, a_World); // Get topmost obsidian block as reference for all other checks; we don't know meta yet, so NULL
		int X1 = X + 1, Z1 = Z + 1, X2 = X - 1, Z2 = Z - 1; // Duplicate XZ values, add/subtract one as we've checked the original already the line above

		if (MaxY == NULL) // Oh noes! Not a portal coordinate :(
		{
			m_AirBlocks.clear();
			return;
		}

		if (!FindPortalSliceX(X1, X2, Y, Z, MaxY, a_World))
		{
			if (!FindPortalSliceZ(X, Y, Z1, Z2, MaxY, a_World))
			{
				m_AirBlocks.clear();
				return;
			}
			else { FindObsidianCeiling(X, Y, Z, 2, a_World); } // This because we now know meta and need to add the air blocks to the vector
		}
		else { FindObsidianCeiling(X, Y, Z, 1, a_World); } // See above

		for (AirBlocks::iterator r = m_AirBlocks.begin(); r != m_AirBlocks.end(); r++)
		{
			sAirBlocksList & Change = *r;
			a_World->SetBlock(Change.a_BlockPos.x, Change.a_BlockPos.y, Change.a_BlockPos.z, E_BLOCK_NETHER_PORTAL, Change.Direction);
		}

		m_AirBlocks.clear();
		return;
	}

	bool FindPortalSliceX(int X1, int X2, int Y, int Z, int MaxY, cWorld * a_World)
	{
		bool FoundFrameXP = false, FoundFrameXM = false;
		for (X1; ((a_World->GetBlock(X1, Y, Z) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X1, Y + 1, Z) == E_BLOCK_OBSIDIAN)); X1++) // Check XP for obsidian blocks, exempting corners
		{
			int Value = FindObsidianCeiling(X1, Y, Z, 1, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X1, Y + 1, Z, 1, a_World, MaxY); // For corners without obsidian
			if ((Value == NULL - 1) || (ValueTwo == NULL - 1)) // FindObsidianCeiling returns NULL - 1 upon frame-find
			{
				FoundFrameXP = true; // Found a frame border in this direction, proceed in other direction (don't go further)
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY)) // Make sure that there is a valid portal 'slice'
			{
				m_AirBlocks.clear();
				return false; // Not valid slice, no portal can be formed
			}
		}
		for (X2; ((a_World->GetBlock(X2, Y, Z) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X2, Y + 1, Z) == E_BLOCK_OBSIDIAN)); X2--) // Go the other direction (XM)
		{
			int Value = FindObsidianCeiling(X2, Y, Z, 1, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X2, Y + 1, Z, 1, a_World, MaxY);
			if ((Value == NULL - 1) || (ValueTwo == NULL - 1))
			{
				FoundFrameXM = true;
				break;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				m_AirBlocks.clear();
				return false;
			}
		}
		return (FoundFrameXP && FoundFrameXM);
	}

	bool FindPortalSliceZ(int X, int Y, int Z1, int Z2, int MaxY, cWorld * a_World)
	{
		bool FoundFrameZP = false, FoundFrameZM = false;
		for (Z1; ((a_World->GetBlock(X, Y, Z1) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X, Y + 1, Z1) == E_BLOCK_OBSIDIAN)); Z1++)
		{
			int Value = FindObsidianCeiling(X, Y, Z1, 2, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X, Y + 1, Z1, 2, a_World, MaxY);
			if ((Value == NULL - 1) || (ValueTwo == NULL - 1))
			{
				FoundFrameZP = true;
				continue;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				m_AirBlocks.clear();
				return false;
			}
		}
		for (Z2; ((a_World->GetBlock(X, Y, Z2) == E_BLOCK_OBSIDIAN) || (a_World->GetBlock(X, Y + 1, Z2) == E_BLOCK_OBSIDIAN)); Z2--)
		{
			int Value = FindObsidianCeiling(X, Y, Z2, 2, a_World, MaxY);
			int ValueTwo = FindObsidianCeiling(X, Y + 1, Z2, 2, a_World, MaxY);
			if ((Value == NULL - 1) || (ValueTwo == NULL - 1))
			{
				FoundFrameZM = true;
				continue;
			}
			else if ((Value != MaxY) && (ValueTwo != MaxY))
			{
				m_AirBlocks.clear();
				return false;
			}
		}
		return (FoundFrameZP && FoundFrameZM);
	}
};




