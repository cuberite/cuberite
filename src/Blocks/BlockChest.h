
#pragma once

#include "BlockEntity.h"
#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "MetaRotator.h"





class cBlockChestHandler :
	public cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>
{
public:
	cBlockChestHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>(a_BlockType)
	{
	}

	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		
		// Is there a doublechest already next to this block?
		if (!CanBeAt(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ))
		{
			// Yup, cannot form a triple-chest, refuse:
			return false;
		}
		
		// Check if this forms a doublechest, if so, need to adjust the meta:
		cBlockArea Area;
		if (!Area.Read(&a_ChunkInterface, a_BlockX - 1, a_BlockX + 1, a_BlockY, a_BlockY, a_BlockZ - 1, a_BlockZ + 1))
		{
			return false;
		}
		double yaw = a_Player->GetYaw();
		if (
			(Area.GetRelBlockType(0, 0, 1) == m_BlockType) ||
			(Area.GetRelBlockType(2, 0, 1) == m_BlockType)
		)
		{
			a_BlockMeta = ((yaw >= -90) && (yaw < 90)) ? 2 : 3;
			return true;
		}
		if (
			(Area.GetRelBlockType(0, 0, 1) == m_BlockType) ||
			(Area.GetRelBlockType(2, 0, 1) == m_BlockType)
		)
		{
			// FIXME: This is unreachable, as the condition is the same as the above one
			a_BlockMeta = (yaw < 0) ? 4 : 5;
			return true;
		}
		
		// Single chest, get meta from rotation only
		a_BlockMeta = RotationToMetaData(yaw);
		return true;
	}
	

	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override
	{
		// Check if this forms a doublechest, if so, need to adjust the meta:
		cBlockArea Area;
		if (!Area.Read(&a_ChunkInterface, a_BlockX - 1, a_BlockX + 1, a_BlockY, a_BlockY, a_BlockZ - 1, a_BlockZ + 1))
		{
			return;
		}
		
		double rot = a_Player->GetYaw();  // FIXME: Rename rot to yaw
		// Choose meta from player rotation, choose only between 2 or 3
		NIBBLETYPE NewMeta = ((rot >= -90) && (rot < 90)) ? 2 : 3;
		if (
			CheckAndAdjustNeighbor(a_ChunkInterface, Area, 0, 1, NewMeta) ||
			CheckAndAdjustNeighbor(a_ChunkInterface, Area, 2, 1, NewMeta)
		)
		{
			// Forming a double chest in the X direction
			return;
		}
		// Choose meta from player rotation, choose only between 4 or 5
		NewMeta = (rot < 0) ? 4 : 5;
		if (
			CheckAndAdjustNeighbor(a_ChunkInterface, Area, 1, 0, NewMeta) ||
			CheckAndAdjustNeighbor(a_ChunkInterface, Area, 2, 2, NewMeta)
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
	
	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		return CanBeAt(a_ChunkInterface, BlockX, a_RelY, BlockZ);
	}

	
	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		cBlockArea Area;
		if (!Area.Read(&a_ChunkInterface, a_BlockX - 2, a_BlockX + 2, a_BlockY, a_BlockY, a_BlockZ - 2, a_BlockZ + 2))
		{
			// Cannot read the surroundings, probably at the edge of loaded chunks. Disallow.
			return false;
		}
		
		int NumChestNeighbors = 0;
		if (Area.GetRelBlockType(1, 0, 2) == m_BlockType)
		{
			if (
				(Area.GetRelBlockType(0, 0, 2) == m_BlockType) ||
				(Area.GetRelBlockType(1, 0, 1) == m_BlockType) ||
				(Area.GetRelBlockType(1, 0, 3) == m_BlockType)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlockType(3, 0, 2) == m_BlockType)
		{
			if (
				(Area.GetRelBlockType(4, 0, 2) == m_BlockType) ||
				(Area.GetRelBlockType(3, 0, 1) == m_BlockType) ||
				(Area.GetRelBlockType(3, 0, 3) == m_BlockType)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlockType(2, 0, 1) == m_BlockType)
		{
			if (
				(Area.GetRelBlockType(2, 0, 0) == m_BlockType) ||
				(Area.GetRelBlockType(1, 0, 1) == m_BlockType) ||
				(Area.GetRelBlockType(3, 0, 1) == m_BlockType)
			)
			{
				// Already a doublechest neighbor, disallow:
				return false;
			}
			NumChestNeighbors += 1;
		}
		if (Area.GetRelBlockType(2, 0, 3) == m_BlockType)
		{
			if (
				(Area.GetRelBlockType(2, 0, 4) == m_BlockType) ||
				(Area.GetRelBlockType(1, 0, 3) == m_BlockType) ||
				(Area.GetRelBlockType(3, 0, 3) == m_BlockType)
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
	bool CheckAndAdjustNeighbor(cChunkInterface & a_ChunkInterface, const cBlockArea & a_Area, int a_RelX, int a_RelZ, NIBBLETYPE a_NewMeta)
	{
		if (a_Area.GetRelBlockType(a_RelX, 0, a_RelZ) != m_BlockType)
		{
			return false;
		}
		a_ChunkInterface.SetBlockMeta(a_Area.GetOriginX() + a_RelX, a_Area.GetOriginY(), a_Area.GetOriginZ() + a_RelZ, a_NewMeta);
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}
} ;




