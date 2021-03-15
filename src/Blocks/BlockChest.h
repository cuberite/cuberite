
#pragma once

#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "Mixins.h"





class cBlockChestHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		// Cannot place right next to double-chest:
		if (!CanBeAt(a_ChunkInterface, a_PlacedBlockPos))
		{
			// Yup, cannot form a triple-chest, refuse:
			return false;
		}

		// Try to read double-chest information:
		cBlockArea Area;
		if (!Area.Read(a_ChunkInterface, a_PlacedBlockPos - Vector3i(1, 0, 1), a_PlacedBlockPos + Vector3i(1, 0, 1)))
		{
			return false;
		}

		// Get meta as if this was a single-chest:
		if (!Super::GetPlacementBlockTypeMeta(a_ChunkInterface, a_Player, a_PlacedBlockPos, a_ClickedBlockFace, a_CursorPos, a_Block))
		{
			return false;
		}

		// Check if this forms a doublechest, if so, need to adjust the meta:
		double yaw = a_Player.GetYaw();
		if (
			(Area.GetRelBlock(0, 0, 1).Type() == m_BlockType) ||
			(Area.GetRelBlock(2, 0, 1).Type() == m_BlockType)
		)
		{
			a_BlockMeta = ((yaw >= -90) && (yaw < 90)) ? 2 : 3;
			return true;
		}
		if (
			(Area.GetRelBlock(1, 0, 0).Type() == m_BlockType) ||
			(Area.GetRelBlock(1, 0, 2).Type() == m_BlockType)
		)
		{
			a_BlockMeta = (yaw < 0) ? 4 : 5;
			return true;
		}


		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		auto BlockPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		return CanBeAt(a_ChunkInterface, BlockPos);
	}





	bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_BlockPos) const
	{
		cBlockArea Area;
		if (!Area.Read(a_ChunkInterface, a_BlockPos - Vector3i(2, 0, 2), a_BlockPos + Vector3i(2, 0, 2)))
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





	/** If there's a chest in the a_Area in the specified coords, modifies its meta to a_NewMeta and returns true. */
	bool CheckAndAdjustNeighbor(cChunkInterface & a_ChunkInterface, const cBlockArea & a_Area, int a_RelX, int a_RelZ, NIBBLETYPE a_NewMeta) const
	{
		if (a_Area.GetRelBlockType(a_RelX, 0, a_RelZ) != m_BlockType)
		{
			return false;
		}
		a_ChunkInterface.SetBlockMeta(a_Area.GetOriginX() + a_RelX, a_Area.GetOriginY(), a_Area.GetOriginZ() + a_RelZ, a_NewMeta);
		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;




