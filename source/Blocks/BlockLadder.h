
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockLadderHandler :
	public cBlockHandler
{
public:
	cBlockLadderHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}	


	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (!LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			a_BlockFace = FindSuitableBlockFace(a_World, a_BlockX, a_BlockY, a_BlockZ);
			
			if (a_BlockFace == BLOCK_FACE_BOTTOM)
			{
				return false;
			}
		}

		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetaData(a_BlockFace);
		return true;
	}


	static NIBBLETYPE DirectionToMetaData(char a_Direction)  // tolua_export
	{  // tolua_export
		switch (a_Direction)
		{
			case 0x2: return 0x2;
			case 0x3: return 0x3;
			case 0x4: return 0x4;
			case 0x5: return 0x5;
			default:  return 0x2;
		}
	}  // tolua_export


	static char MetaDataToDirection(NIBBLETYPE a_MetaData)  // tolua_export
	{														// tolua_export
		switch (a_MetaData)
		{
			case 0x2: return 0x2;
			case 0x3: return 0x3;
			case 0x4: return 0x4;
			case 0x5: return 0x5;
			default:  return 0x2;
		}
	}  // tolua_export


	/// Finds a suitable Direction for the Ladder. Returns BLOCK_FACE_BOTTOM on failure
	static char FindSuitableBlockFace(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int Face = 2; Face <= 5; Face++)
		{
			if (LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, Face))
			{
				return Face;
			}
		}
		return BLOCK_FACE_BOTTOM;
	}
	

	static bool LadderCanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
	{
		if ((a_BlockFace == BLOCK_FACE_BOTTOM) || (a_BlockFace == BLOCK_FACE_TOP))
		{
			return false;
		}

		AddFaceDirection( a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);

		return g_BlockIsSolid[a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ)];
	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		// TODO: Use AdjustCoordsByMeta(), then cChunk::UnboundedRelGetBlock() and finally some comparison
		char BlockFace = MetaDataToDirection(a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		return LadderCanBePlacedAt(a_Chunk.GetWorld(), BlockX, a_RelY, BlockZ, BlockFace);
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




