#pragma once

#include "BlockHandler.h"





class cBlockVineHandler final :
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
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		// TODO: Disallow placement where the vine doesn't attach to something properly
		BLOCKTYPE BlockType = 0;
		NIBBLETYPE BlockMeta;
		a_ChunkInterface.GetBlockTypeMeta(a_PlacedBlockPos, BlockType, BlockMeta);
		if (BlockType == m_BlockType)
		{
			a_BlockMeta = BlockMeta | DirectionToMetaData(a_ClickedBlockFace);
		}
		else
		{
			a_BlockMeta = DirectionToMetaData(a_ClickedBlockFace);
		}
		a_BlockType = m_BlockType;
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Only drops self when using shears, otherwise drops nothing:
		if ((a_Tool == nullptr) || (a_Tool->m_ItemType != E_ITEM_SHEARS))
		{
			return {};
		}
		return cItem(E_BLOCK_VINES, 1, 0);
	}





	static NIBBLETYPE DirectionToMetaData(char a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_NORTH: return 0x1;
			case BLOCK_FACE_SOUTH: return 0x4;
			case BLOCK_FACE_WEST:  return 0x8;
			case BLOCK_FACE_EAST:  return 0x2;
			default: return 0x0;
		}
	}





	static char MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0x1: return BLOCK_FACE_NORTH;
			case 0x4: return BLOCK_FACE_SOUTH;
			case 0x8: return BLOCK_FACE_WEST;
			case 0x2: return BLOCK_FACE_EAST;
			default:  return BLOCK_FACE_TOP;
		}
	}





	/** Returns true if the specified block type is good for vines to attach to */
	static bool IsBlockAttachable(BLOCKTYPE a_BlockType)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_CHEST:
			case E_BLOCK_ENDER_CHEST:
			case E_BLOCK_GLASS:
			case E_BLOCK_PISTON:
			case E_BLOCK_PISTON_EXTENSION:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_STAINED_GLASS:
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_TRAPPED_CHEST:
			{
				// You can't attach a vine to this solid blocks.
				return false;
			}
			default:
			{
				return cBlockInfo::IsSolid(a_BlockType);
			}
		}
	}





	/** Returns the meta that has the maximum allowable sides of the vine, given the surroundings */
	static NIBBLETYPE GetMaxMeta(cChunk & a_Chunk, Vector3i a_RelPos)
	{
		static const struct
		{
			int x, z;
			NIBBLETYPE Bit;
		} Coords[] =
		{
			{ 0,  1, 1},  // south, ZP
			{-1,  0, 2},  // west,  XM
			{ 0, -1, 4},  // north, ZM
			{ 1,  0, 8},  // east,  XP
		} ;
		NIBBLETYPE res = 0;
		for (auto & Coord : Coords)
		{
			BLOCKTYPE  BlockType;
			NIBBLETYPE BlockMeta;
			auto checkPos = a_RelPos.addedXZ(Coord.x, Coord.z);
			if (
				a_Chunk.UnboundedRelGetBlock(checkPos.x, checkPos.y, checkPos.z, BlockType, BlockMeta) &&
				IsBlockAttachable(BlockType)
			)
			{
				res |= Coord.Bit;
			}
		}
		return res;
	}





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{

		const auto a_RelPos = a_Chunk.AbsoluteToRelative(a_BlockPos);
		NIBBLETYPE CurMeta = a_Chunk.GetMeta(a_RelPos);
		NIBBLETYPE MaxMeta = GetMaxMeta(a_Chunk, a_RelPos);

		// Check if vine above us, add its meta to MaxMeta
		if ((a_RelPos.y < cChunkDef::Height - 1) && (a_Chunk.GetBlock(a_RelPos.addedY(1)) == m_BlockType))
		{
			MaxMeta |= a_Chunk.GetMeta(a_RelPos.addedY(1));
		}

		NIBBLETYPE Common = CurMeta & MaxMeta;  // Neighbors that we have and are legal
		if (Common != CurMeta)
		{
			// There is a neighbor missing, need to update the meta or even destroy the block
			bool HasTop = (a_RelPos.y < cChunkDef::Height - 1) && IsBlockAttachable(a_Chunk.GetBlock(a_RelPos.addedY(1)));
			if ((Common == 0) && !HasTop)
			{
				// The vine just lost all its support, destroy the block:
				if (DoesDropOnUnsuitable())
				{
					a_ChunkInterface.DropBlockAsPickups(a_Chunk.RelativeToAbsolute(a_RelPos));
				}
				a_Chunk.SetBlock(a_RelPos, E_BLOCK_AIR, 0);
				return false;
			}
			a_Chunk.SetBlock(a_RelPos, m_BlockType, Common);
		}

		return false;
		});
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) const override
	{
		return true;
	}





	virtual bool DoesDropOnUnsuitable(void) const override
	{
		return false;
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		UNUSED(a_ChunkInterface);
		UNUSED(a_WorldInterface);

		// Vine cannot grow down if at the bottom:
		auto GrowPos = a_RelPos.addedY(-1);
		if (!cChunkDef::IsValidHeight(GrowPos.y))
		{
			return;
		}

		// Grow one block down, if possible:
		BLOCKTYPE Block;
		a_Chunk.UnboundedRelGetBlockType(GrowPos, Block);
		if (Block == E_BLOCK_AIR)
		{
			auto WorldPos = a_Chunk.RelativeToAbsolute(GrowPos);
			if (!a_PluginInterface.CallHookBlockSpread(WorldPos.x, WorldPos.y, WorldPos.z, ssVineSpread))
			{
				a_Chunk.UnboundedRelSetBlock(GrowPos, E_BLOCK_VINES, a_Chunk.GetMeta(a_RelPos));
			}
		}
	}





	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) const override
	{
		return ((a_Meta >> 1) | (a_Meta << 3)) & 0x0f;  // Rotate bits to the right
	}





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) const override
	{
		return ((a_Meta << 1) | (a_Meta >> 3)) & 0x0f;  // Rotate bits to the left
	}





	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) const override
	{
		// Bits 2 and 4 stay, bits 1 and 3 swap
		return static_cast<NIBBLETYPE>((a_Meta & 0x0a) | ((a_Meta & 0x01) << 2) | ((a_Meta & 0x04) >> 2));
	}





	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) const override
	{
		// Bits 1 and 3 stay, bits 2 and 4 swap
		return static_cast<NIBBLETYPE>((a_Meta & 0x05) | ((a_Meta & 0x02) << 2) | ((a_Meta & 0x08) >> 2));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




