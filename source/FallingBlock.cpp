#include "Globals.h"

#include "FallingBlock.h"
#include "World.h"
#include "ClientHandle.h"
#include "Simulator/SandSimulator.h"





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





void cFallingBlock::Tick(float a_Dt, MTRand & a_TickRandom)
{
	float MilliDt = a_Dt * 0.001f;
	m_Speed.y -= MilliDt * 9.8f;
	m_Pos.y += m_Speed.y * MilliDt;

	// GetWorld()->BroadcastTeleportEntity(*this);  // Test position
	
	int BlockX = (int)m_OriginalPosition.x;
	int BlockY = (int)(m_Pos.y - 0.5);
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
	
	if (BlockY < cChunkDef::Height - 1)
	{
		BLOCKTYPE BlockBelow = GetWorld()->GetBlock(BlockX, BlockY, BlockZ);
		if (
			cSandSimulator::DoesBreakFallingThrough(BlockBelow) ||  // Fallen onto a block that breaks this into pickups (e. g. half-slab)
			!cSandSimulator::CanContinueFallThrough(BlockBelow)     // Fallen onto a solid block
		)
		{
			cSandSimulator::FinishFalling(m_World, BlockX, BlockY + 1, BlockZ, m_BlockType, m_BlockMeta);
			Destroy();
			return;
		}
	}
}




