#pragma once

#include "BlockHandler.h"





class cBlockVineHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	static const NIBBLETYPE VINE_LOST_SUPPORT = 16;
	static const NIBBLETYPE VINE_UNCHANGED = 17;


	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		return GetMaxMeta(a_Chunk, a_Position, a_Meta) != VINE_LOST_SUPPORT;
	}


	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Only drops self when using shears, otherwise drops nothing:
		if ((a_Tool == nullptr) || (a_Tool->m_ItemType != E_ITEM_SHEARS))
		{
			return {};
		}
		return cItem(E_BLOCK_VINES, 1, 0);
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





	/** Returns the meta that has the maximum allowable sides of the vine, given the surroundings and current vine meta.
	Returns special values for a vine that can continue to exist unchanged, or must die completely. */
	static NIBBLETYPE GetMaxMeta(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_CurrentMeta)
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

		NIBBLETYPE MaxMeta = 0;
		for (auto & Coord : Coords)
		{
			BLOCKTYPE  BlockType;
			NIBBLETYPE BlockMeta;
			auto checkPos = a_Position.addedXZ(Coord.x, Coord.z);
			if (
				a_Chunk.UnboundedRelGetBlock(checkPos.x, checkPos.y, checkPos.z, BlockType, BlockMeta) &&
				IsBlockAttachable(BlockType)
			)
			{
				MaxMeta |= Coord.Bit;
			}
		}

		// Check if vine above us, add its meta to MaxMeta:
		if ((a_Position.y < cChunkDef::Height - 1) && (a_Chunk.GetBlock(a_Position.addedY(1)) == E_BLOCK_VINES))
		{
			MaxMeta |= a_Chunk.GetMeta(a_Position.addedY(1));
		}

		NIBBLETYPE Common = a_CurrentMeta & MaxMeta;  // Neighbors that we have and are legal.
		if (Common != a_CurrentMeta)
		{
			bool HasTop = (a_Position.y < (cChunkDef::Height - 1)) && IsBlockAttachable(a_Chunk.GetBlock(a_Position.addedY(1)));
			if ((Common == 0) && !HasTop)  // Meta equals 0 also means top. Make a last-ditch attempt to save the vine.
			{
				return VINE_LOST_SUPPORT;
			}

			return Common;
		}

		return VINE_UNCHANGED;
	}





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{

		const auto Position = cChunkDef::AbsoluteToRelative(a_BlockPos);
		const auto MaxMeta = GetMaxMeta(a_Chunk, Position, a_Chunk.GetMeta(Position));

		if (MaxMeta == VINE_UNCHANGED)
		{
			return false;
		}

		// There is a neighbor missing, need to update the meta or even destroy the block.

		if (MaxMeta == VINE_LOST_SUPPORT)
		{
			// The vine just lost all its support, destroy the block:
			a_Chunk.SetBlock(Position, E_BLOCK_AIR, 0);
		}
		else
		{
			// It lost some of its support, set it to what remains (SetBlock to notify neighbors):
			a_Chunk.SetBlock(Position, E_BLOCK_VINES, MaxMeta);
		}

		return false;
		});
	}





	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, const NIBBLETYPE a_Meta, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		return !a_ClickedDirectly || (a_HeldItem.m_ItemType != m_BlockType);
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




