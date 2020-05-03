#include "Globals.h"

#include "FallingBlock.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "../Simulator/SandSimulator.h"
#include "../Chunk.h"





cFallingBlock::cFallingBlock(Vector3d a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta):
	Super(etFallingBlock, a_Position, 0.98, 0.98),
	m_BlockType(a_BlockType),
	m_BlockMeta(a_BlockMeta)
{
	SetGravity(-16.0f);
	SetAirDrag(0.02f);
}





void cFallingBlock::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnEntity(*this);
}





void cFallingBlock::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	// GetWorld()->BroadcastTeleportEntity(*this);  // Test position

	int BlockX = POSX_TOINT;
	int BlockY = static_cast<int>(GetPosY() - 0.5);
	int BlockZ = POSZ_TOINT;

	if (BlockY < 0)
	{
		// Fallen out of this world, just continue falling until out of sight, then destroy:
		if (BlockY < VOID_BOUNDARY)
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

	BLOCKTYPE BlockBelow = a_Chunk.GetBlock(BlockX - a_Chunk.GetPosX() * cChunkDef::Width, BlockY, BlockZ - a_Chunk.GetPosZ() * cChunkDef::Width);
	NIBBLETYPE BelowMeta = a_Chunk.GetMeta(BlockX - a_Chunk.GetPosX() * cChunkDef::Width, BlockY, BlockZ - a_Chunk.GetPosZ() * cChunkDef::Width);
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
		/*
		FLOGD(
			"Sand: Checked below at {0} (rel {1}), it's {2}, finishing the fall.",
			Vector3i{BlockX, BlockY, BlockZ},
			cChunkDef::AbsoluteToRelative({BlockX, BlockY, BlockZ}, {a_Chunk.GetPosX(), a_Chunk.GetPosZ()}),
			ItemTypeToString(BlockBelow)
		);
		*/

		if (BlockY < cChunkDef::Height - 1)
		{
			cSandSimulator::FinishFalling(m_World, BlockX, BlockY + 1, BlockZ, m_BlockType, m_BlockMeta);
		}
		Destroy();
		return;
	}
	else if ((m_BlockType == E_BLOCK_CONCRETE_POWDER) && IsBlockWater(BlockBelow))
	{
		// Concrete powder falling into water solidifies on the first water it touches
		cSandSimulator::FinishFalling(m_World, BlockX, BlockY, BlockZ, E_BLOCK_CONCRETE, m_BlockMeta);
		Destroy();
		return;
	}

	float MilliDt = a_Dt.count() * 0.001f;
	AddSpeedY(MilliDt * -9.8f);
	AddPosition(GetSpeed() * MilliDt);

	// If not static (one billionth precision) broadcast movement
	if ((fabs(GetSpeedX()) > std::numeric_limits<double>::epsilon()) || (fabs(GetSpeedZ()) > std::numeric_limits<double>::epsilon()))
	{
		BroadcastMovementUpdate();
	}
}




