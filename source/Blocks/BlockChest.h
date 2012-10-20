
#pragma once

#include "BlockEntity.h"
#include "../World.h"
#include "../Piston.h"
#include "../Player.h"





class cBlockChestHandler :
	public cBlockEntityHandler
{
public:
	cBlockChestHandler(BLOCKTYPE a_BlockType)
		: cBlockEntityHandler(a_BlockType)
	{
	}

	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cPiston::RotationPitchToMetaData(a_Player->GetRotation(), 0));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}
	
	
	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
	
	
	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		cBlockArea Area;
		if (!Area.Read(a_World, a_BlockX - 2, a_BlockX + 2, a_BlockY, a_BlockY, a_BlockZ - 2, a_BlockZ + 2))
		{
			// Cannot read the surroundings, probably at the edge of loaded chunks. Disallow.
			return false;
		}
		
		int NumChestNeighbors = 0;
		if (Area.GetRelBlockType(1, 0, 2) == E_BLOCK_CHEST)
		{
			if (
				(Area.GetRelBlockType(0, 0, 2) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(1, 0, 1) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(1, 0, 3) == E_BLOCK_CHEST)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlockType(3, 0, 2) == E_BLOCK_CHEST)
		{
			if (
				(Area.GetRelBlockType(4, 0, 2) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(3, 0, 1) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(3, 0, 3) == E_BLOCK_CHEST)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlockType(2, 0, 1) == E_BLOCK_CHEST)
		{
			if (
				(Area.GetRelBlockType(2, 0, 0) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(1, 0, 1) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(3, 0, 1) == E_BLOCK_CHEST)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlockType(2, 0, 3) == E_BLOCK_CHEST)
		{
			if (
				(Area.GetRelBlockType(2, 0, 4) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(1, 0, 3) == E_BLOCK_CHEST) ||
				(Area.GetRelBlockType(3, 0, 3) == E_BLOCK_CHEST)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		return (NumChestNeighbors < 2);
	}
	
	
} ;




