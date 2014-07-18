
#pragma once

#include "BlockHandler.h"
#include "../Entities/Player.h"
#include "Chunk.h"





class cBlockWallSignHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;

public:
	cBlockWallSignHandler(BLOCKTYPE a_BlockType) :
		super(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SIGN, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}


	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override
	{
		a_Player->GetClientHandle()->SendEditSign(a_BlockX, a_BlockY, a_BlockZ);
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		int BlockX = (a_Chunk.GetPosX() * cChunkDef::Width) + a_RelX;
		int BlockZ = (a_Chunk.GetPosZ() * cChunkDef::Width) + a_RelZ;
		GetBlockCoordsBehindTheSign(a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ), BlockX, BlockZ);

		return (cBlockInfo::IsSolid(a_ChunkInterface.GetBlock(BlockX, a_RelY, BlockZ)));
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
			case 0x2: return 0x2;
			case 0x3: return 0x3;
			case 0x4: return 0x4;
			case 0x5: return 0x5;
			default:
			{
				break;
			}
		}
		return 0x2;
	}
} ;




