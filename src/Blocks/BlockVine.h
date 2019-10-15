#pragma once

#include "BlockHandler.h"





class cBlockVineHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockVineHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// TODO: Disallow placement where the vine doesn't attach to something properly
		BLOCKTYPE BlockType = 0;
		NIBBLETYPE BlockMeta;
		a_ChunkInterface.GetBlockTypeMeta({a_BlockX, a_BlockY, a_BlockZ}, BlockType, BlockMeta);
		if (BlockType == m_BlockType)
		{
			a_BlockMeta = BlockMeta | DirectionToMetaData(a_BlockFace);
		}
		else
		{
			a_BlockMeta = DirectionToMetaData(a_BlockFace);
		}
		a_BlockType = m_BlockType;
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
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
	NIBBLETYPE GetMaxMeta(cChunk & a_Chunk, Vector3i a_RelPos)
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





	virtual void Check(
		cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface,
		Vector3i a_RelPos,
		cChunk & a_Chunk
	) override
	{
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
				return;
			}
			a_Chunk.SetBlock(a_RelPos, m_BlockType, Common);
		}
		else
		{
			auto absPos = a_Chunk.RelativeToAbsolute(a_RelPos);
			a_Chunk.GetWorld()->GetSimulatorManager()->WakeUp(absPos, &a_Chunk);
		}
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		return true;
	}





	virtual bool DoesDropOnUnsuitable(void) override
	{
		return false;
	}





	virtual void OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		UNUSED(a_ChunkInterface);
		UNUSED(a_WorldInterface);

		// Vine cannot grow down if at the bottom:
		if (a_RelY < 1)
		{
			return;
		}

		// Grow one block down, if possible:
		BLOCKTYPE Block;
		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY - 1, a_RelZ, Block);
		if (Block == E_BLOCK_AIR)
		{
			if (!a_BlockPluginInterface.CallHookBlockSpread(a_RelX + a_Chunk.GetPosX() * cChunkDef::Width, a_RelY - 1, a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width, ssVineSpread))
			{
				a_Chunk.UnboundedRelSetBlock(a_RelX, a_RelY - 1, a_RelZ, E_BLOCK_VINES, a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
			}
		}
	}





	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		return ((a_Meta >> 1) | (a_Meta << 3)) & 0x0f;  // Rotate bits to the right
	}





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		return ((a_Meta << 1) | (a_Meta >> 3)) & 0x0f;  // Rotate bits to the left
	}





	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Bits 2 and 4 stay, bits 1 and 3 swap
		return static_cast<NIBBLETYPE>((a_Meta & 0x0a) | ((a_Meta & 0x01) << 2) | ((a_Meta & 0x04) >> 2));
	}





	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Bits 1 and 3 stay, bits 2 and 4 swap
		return static_cast<NIBBLETYPE>((a_Meta & 0x05) | ((a_Meta & 0x02) << 2) | ((a_Meta & 0x08) >> 2));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




