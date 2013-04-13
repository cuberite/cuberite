#include "Globals.h"

#include "FallingBlock.h"
#include "World.h"
#include "ClientHandle.h"
#include "Simulator/SandSimulator.h"
#include "Chunk.h"





cFallingBlock::cFallingBlock(const Vector3i & a_BlockPosition, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) :
	super(etFallingBlock, a_BlockPosition.x + 0.5f, a_BlockPosition.y + 0.5f, a_BlockPosition.z + 0.5f),
	m_BlockType(a_BlockType),
	m_BlockMeta(a_BlockMeta),
	m_OriginalPosition(a_BlockPosition)
{
}





void cFallingBlock::Initialize(cWorld * a_World)
{
	super::Initialize(a_World);
	a_World->BroadcastSpawn(*this);
}





void cFallingBlock::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnFallingBlock(*this);
}





void cFallingBlock::Tick(float a_Dt, cChunk & a_Chunk)
{
	float MilliDt = a_Dt * 0.001f;
	AddSpeedY(MilliDt * -9.8f);
	AddPosY(GetSpeedY() * MilliDt);

	// GetWorld()->BroadcastTeleportEntity(*this);  // Test position
	
	int BlockX = (int)m_OriginalPosition.x;
	int BlockY = (int)(GetPosY() - 0.5);
	int BlockZ = (int)m_OriginalPosition.z;
	
	if (BlockY < 0)
	{
		// Fallen out of this world, just continue falling until out of sight, then destroy:
		if (BlockY < 100)
		{
			Destroy();
		}
		return;
	}
	
	if (BlockY >= cChunkDef::Height)
	{
		// Above the world, just wait for it to fall back down
		return;
	}
	
	int idx = a_Chunk.MakeIndexNoCheck(BlockX - a_Chunk.GetPosX() * cChunkDef::Width, BlockY, BlockZ - a_Chunk.GetPosZ() * cChunkDef::Width);
	BLOCKTYPE BlockBelow = a_Chunk.GetBlock(idx);
	NIBBLETYPE BelowMeta = a_Chunk.GetMeta(idx);
	if (cSandSimulator::DoesBreakFallingThrough(BlockBelow, BelowMeta))
	{
		// Fallen onto a block that breaks this into pickups (e. g. half-slab)
		// Must finish the fall with coords one below the block:
		cSandSimulator::FinishFalling(m_World, BlockX, BlockY, BlockZ, m_BlockType, m_BlockMeta);
		Destroy();
		return;
	}
	else if (!cSandSimulator::CanContinueFallThrough(BlockBelow))
	{
		// Fallen onto a solid block
		cSandSimulator::FinishFalling(m_World, BlockX, BlockY + 1, BlockZ, m_BlockType, m_BlockMeta);
		Destroy();
		return;
	}
}




