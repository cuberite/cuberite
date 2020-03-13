
#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"





class cBlockWallSignHandler:
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockWallSignHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_SIGN, 1, 0);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		int BlockX = (a_Chunk.GetPosX() * cChunkDef::Width) + a_RelX;
		int BlockZ = (a_Chunk.GetPosZ() * cChunkDef::Width) + a_RelZ;
		GetBlockCoordsBehindTheSign(a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ), BlockX, BlockZ);
		BLOCKTYPE Type = a_ChunkInterface.GetBlock({BlockX, a_RelY, BlockZ});

		return ((Type == E_BLOCK_WALLSIGN) || (Type == E_BLOCK_SIGN_POST) || cBlockInfo::IsSolid(Type));
	}





	static void GetBlockCoordsBehindTheSign(NIBBLETYPE a_BlockMeta, int & a_BlockX, int & a_BlockZ)
	{
		switch (a_BlockMeta)
		{
			case 2:  a_BlockZ++; break;
			case 3:  a_BlockZ--; break;
			case 4:  a_BlockX++; break;
			case 5:  a_BlockX--; break;
			default: break;
		}
	}





	static NIBBLETYPE DirectionToMetaData(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_ZM: return 0x02;
			case BLOCK_FACE_ZP: return 0x03;
			case BLOCK_FACE_XM: return 0x04;
			case BLOCK_FACE_XP: return 0x05;
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YP:
			case BLOCK_FACE_YM:
			{
				break;
			}
		}
		return 0x02;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;




