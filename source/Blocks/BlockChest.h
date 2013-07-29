
#pragma once

#include "BlockEntity.h"
#include "../World.h"
#include "../Player.h"





class cBlockChestHandler :
	public cBlockEntityHandler
{
public:
	cBlockChestHandler(BLOCKTYPE a_BlockType)
		: cBlockEntityHandler(a_BlockType)
	{
	}

	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		
		// Is there a doublechest already next to this block?
		if (!CanBeAt(a_World, a_BlockX, a_BlockY, a_BlockZ))
		{
			// Yup, cannot form a triple-chest, refuse:
			return false;
		}
		
		// Check if this forms a doublechest, if so, need to adjust the meta:
		cBlockArea Area;
		if (!Area.Read(a_World, a_BlockX - 1, a_BlockX + 1, a_BlockY, a_BlockY, a_BlockZ - 1, a_BlockZ + 1))
		{
			return false;
		}
		double rot = a_Player->GetRotation();
		if (
			(Area.GetRelBlockType(0, 0, 1) == E_BLOCK_CHEST) ||
			(Area.GetRelBlockType(2, 0, 1) == E_BLOCK_CHEST)
		)
		{
			a_BlockMeta = ((rot >= -90) && (rot < 90)) ? 2 : 3;
			return true;
		}
		if (
			(Area.GetRelBlockType(0, 0, 1) == E_BLOCK_CHEST) ||
			(Area.GetRelBlockType(2, 0, 1) == E_BLOCK_CHEST)
		)
		{
			a_BlockMeta = (rot < 0) ? 4 : 5;
			return true;
		}
		
		// Single chest, get meta from rotation only
		a_BlockMeta = RotationToMetaData(rot);
		return true;
	}
	

	virtual void OnPlacedByPlayer(
		cWorld * a_World, cPlayer * a_Player, 
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override
	{
		// Check if this forms a doublechest, if so, need to adjust the meta:
		cBlockArea Area;
		if (!Area.Read(a_World, a_BlockX - 1, a_BlockX + 1, a_BlockY, a_BlockY, a_BlockZ - 1, a_BlockZ + 1))
		{
			return;
		}
		
		double rot = a_Player->GetRotation();
		// Choose meta from player rotation, choose only between 2 or 3
		NIBBLETYPE NewMeta = ((rot >= -90) && (rot < 90)) ? 2 : 3;
		if (
			CheckAndAdjustNeighbor(a_World, Area, 0, 1, NewMeta) ||
			CheckAndAdjustNeighbor(a_World, Area, 2, 1, NewMeta)
		)
		{
			// Forming a double chest in the X direction
			return;
		}
		// Choose meta from player rotation, choose only between 4 or 5
		NewMeta = (rot < 0) ? 4 : 5;
		if (
			CheckAndAdjustNeighbor(a_World, Area, 1, 0, NewMeta) ||
			CheckAndAdjustNeighbor(a_World, Area, 2, 2, NewMeta)
		)
		{
			// Forming a double chest in the Z direction
			return;
		}
		
		// Single chest, no further processing needed
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
	
	
	/// Translates player rotation when placing a chest into the chest block metadata. Valid for single chests only
	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45;  // So its not aligned with axis

		if (a_Rotation > 360.f)
		{
			a_Rotation -= 360.f;
		}
		if ((a_Rotation >= 0.f) && (a_Rotation < 90.f))
		{
			return 0x4;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x5;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x2;
		}
		else
		{
			return 0x3;
		}
	}
	
	
	/// If there's a chest in the a_Area in the specified coords, modifies its meta to a_NewMeta and returns true.
	bool CheckAndAdjustNeighbor(cWorld * a_World, const cBlockArea & a_Area, int a_RelX, int a_RelZ, NIBBLETYPE a_NewMeta)
	{
		if (a_Area.GetRelBlockType(a_RelX, 0, a_RelZ) != E_BLOCK_CHEST)
		{
			return false;
		}
		a_World->SetBlockMeta(a_Area.GetOriginX() + a_RelX, a_Area.GetOriginY(), a_Area.GetOriginZ() + a_RelZ, a_NewMeta);
		return true;
	}
} ;




