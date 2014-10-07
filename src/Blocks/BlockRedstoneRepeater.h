
#pragma once

#include "BlockHandler.h"
#include "Chunk.h"
#include "MetaRotator.h"
#include "ChunkInterface.h"



class cBlockRedstoneRepeaterHandler :
	public cMetaRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03, true>
{
public:
	cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03, true>(a_BlockType)
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
		a_BlockMeta = RepeaterRotationToMetaData(a_Player->GetYaw());
		return true;
	}


	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, ((a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) + 0x04) & 0x0f));
	}


	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		UNUSED(a_ChunkInterface);
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}

	inline static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case 0x1: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_ZM;
			case 0x5:
			case 0x6: return BLOCK_FACE_YP;
			case 0x7:
			case 0x0: return BLOCK_FACE_YM;
			default:
			{
				ASSERT(!"Unhandled block meta!");
				return BLOCK_FACE_NONE;
			}
		}
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_REPEATER, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{	
		if ((a_RelY <= 0) || (a_RelY >= cChunkDef::Height))
		{
			return false;
		}

		NIBBLETYPE Meta;
		if (!a_Chunk.UnboundedRelGetBlockMeta(a_RelX, a_RelY, a_RelZ, Meta))
		{
			return false;
		}
		eBlockFace Face = BlockMetaDataToBlockFace(Meta);

		AddFaceDirection(a_RelX, a_RelY, a_RelZ, Face, true);
		BLOCKTYPE BlockIsOn;
		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOn);

		if (
			(
				((BlockIsOn == E_BLOCK_WOODEN_SLAB) && ((Meta & 0x08) == 0x08)) ||
				((BlockIsOn == E_BLOCK_STONE_SLAB) && ((Meta & 0x08) == 0x08))
			) &&
			((a_RelY < cChunkDef::Height) && (Face == BLOCK_FACE_TOP))
		)
		{
			return true;
		}
		else
		{
			return ((a_RelY > 0) && cBlockInfo::IsSolid(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)));
		}
	}


	inline static NIBBLETYPE RepeaterRotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45;  // So its not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}

		if ((a_Rotation >= 0) && (a_Rotation < 90))
		{
			return 0x1;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x3;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x2;
		}
		else
		{
			return 0x0;
		}
	}
} ;




