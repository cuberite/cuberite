
#pragma once

#include "BlockHandler.h"
#include "../Vine.h"





class cBlockVineHandler :
	public cBlockHandler
{
public:
	cBlockVineHandler(BLOCKTYPE a_BlockType)
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
		// TODO: Disallow placement where the vine doesn't attach to something properly
		BLOCKTYPE BlockType = 0;
		NIBBLETYPE BlockMeta;
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, BlockType, BlockMeta);
		if (BlockType == m_BlockType)
		{
			a_BlockMeta = BlockMeta | cVine::DirectionToMetaData(a_BlockFace);
		}
		else
		{
			a_BlockMeta = cVine::DirectionToMetaData(a_BlockFace);
		}
		a_BlockType = m_BlockType;
		return true;
	}


	/// Returns true if the specified block type is good for vines to attach to
	static bool IsBlockAttachable(BLOCKTYPE a_BlockType)
	{
		return (a_BlockType == E_BLOCK_LEAVES) || g_BlockIsSolid[a_BlockType];
	}


	/// Returns the meta that has the maximum allowable sides of the vine, given the surroundings
	NIBBLETYPE GetMaxMeta(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
	{
		static const struct
		{
			int x, z;
			int Bit;
		} Coords[] =
		{
			{ 0,  1, 1},  // south, ZP
			{-1,  0, 2},  // west,  XM
			{ 0, -1, 4},  // north, ZM
			{ 1,  0, 8},  // east,  XP
		} ;
		int res = 0;
		for (int i = 0; i < ARRAYCOUNT(Coords); i++)
		{
			BLOCKTYPE  BlockType;
			NIBBLETYPE BlockMeta;
			if (
				a_Chunk.UnboundedRelGetBlock(a_RelX + Coords[i].x, a_RelY, a_RelZ + Coords[i].z, BlockType, BlockMeta) &&
				IsBlockAttachable(BlockType)
			)
			{
				res |= Coords[i].Bit;
			}
		}
		return res;
	}
	
	
	void Check(int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk) override
	{
		NIBBLETYPE CurMeta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		NIBBLETYPE MaxMeta = GetMaxMeta(a_Chunk, a_RelX, a_RelY, a_RelZ);
		
		// Check if vine above us, add its meta to MaxMeta
		if ((a_RelY < cChunkDef::Height - 1) && (a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ) == m_BlockType))
		{
			MaxMeta |= a_Chunk.GetMeta(a_RelX, a_RelY + 1, a_RelZ);
		}
		
		NIBBLETYPE Common = CurMeta & MaxMeta;  // Neighbors that we have and are legal
		if (Common != CurMeta)
		{
			// There is a neighbor missing, need to update the meta or even destroy the block
			bool HasTop = (a_RelY < cChunkDef::Height - 1) && IsBlockAttachable(a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ));
			if ((Common == 0) && !HasTop)
			{
				// The vine just lost all its support, destroy the block:
				if (DoesDropOnUnsuitable())
				{
					int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
					int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
					DropBlock(a_Chunk.GetWorld(), NULL, BlockX, a_RelY, BlockZ);
				}
				a_Chunk.SetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_AIR, 0);
				return;
			}
			a_Chunk.SetBlock(a_RelX, a_RelY, a_RelZ, m_BlockType, Common);
		}
		else
		{
			// Wake up the simulators for this block:
			int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
			int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
			a_Chunk.GetWorld()->GetSimulatorManager()->WakeUp(BlockX, a_RelY, BlockZ, &a_Chunk);
		}
	}


	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}


	bool DoesDropOnUnsuitable(void)
	{
		return false;
	}
} ;




