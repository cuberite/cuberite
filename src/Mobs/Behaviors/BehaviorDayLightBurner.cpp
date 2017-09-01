#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorDayLightBurner.h"
#include "../Monster.h"
#include "../../Entities/Player.h"
#include "../../Entities/Entity.h"
#include "../../Chunk.h"





void cBehaviorDayLightBurner::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachPostTickBehavior(this);
	m_Parent->GetPathFinder().SetAvoidSunlight(true);
}





void cBehaviorDayLightBurner::PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	int RelY = static_cast<int>(m_Parent->GetPosY());
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		// Outside the world
		return;
	}
	if (!a_Chunk.IsLightValid())
	{
		m_Parent->GetWorld()->QueueLightChunk(m_Parent->GetChunkX(), m_Parent->GetChunkZ());
		return;
	}

	if (!m_Parent->IsOnFire() && WouldBurnAt(m_Parent->GetPosition(), a_Chunk, *m_Parent))
	{
		// Burn for 100 ticks, then decide again
		m_Parent->StartBurning(100);
	}
}




bool cBehaviorDayLightBurner::WouldBurnAt(Vector3d a_Location, cChunk & a_Chunk, cMonster & a_Monster)
{
	int RelY = FloorC(a_Location.y);
	if (RelY <= 0)
	{
		// The mob is about to die, no point in burning
		return false;
	}
	if (RelY >= cChunkDef::Height)
	{
		// Always burn above the world
		return true;
	}

	PREPARE_REL_AND_CHUNK(a_Location, a_Chunk);
	if (!RelSuccess)
	{
		return false;
	}

	if (
		(Chunk->GetBlock(Rel.x, Rel.y, Rel.z) != E_BLOCK_SOULSAND) &&  // Not on soulsand
		(a_Monster.GetWorld()->GetTimeOfDay() < 12000 + 1000) &&              // Daytime
		a_Monster.GetWorld()->IsWeatherSunnyAt(static_cast<int>(a_Monster.GetPosX()), static_cast<int>(a_Monster.GetPosZ()))  // Not raining
	)
	{
		int MobHeight = CeilC(a_Location.y + a_Monster.GetHeight()) - 1;   // The height of the mob head
		if (MobHeight >= cChunkDef::Height)
		{
			return true;
		}
		// Start with the highest block and scan down to just abovethe mob's head.
		// If a non transparent is found, return false (do not burn). Otherwise return true.
		// Note that this loop is not a performance concern as transparent blocks are rare and the loop almost always bailes out
		// instantly.(An exception is e.g. standing under a long column of glass).
		int CurrentBlock = Chunk->GetHeight(Rel.x, Rel.z);
		while (CurrentBlock > MobHeight)
		{
			BLOCKTYPE Block = Chunk->GetBlock(Rel.x, CurrentBlock, Rel.z);
			if (
				// Do not burn if a block above us meets one of the following conditions:
				(!cBlockInfo::IsTransparent(Block)) ||
				(Block == E_BLOCK_LEAVES) ||
				(Block == E_BLOCK_NEW_LEAVES) ||
				(IsBlockWater(Block))
			)
			{
				return false;
			}
			--CurrentBlock;
		}
		return true;

	}
	return false;
}
