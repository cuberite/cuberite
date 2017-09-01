#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BehaviorWanderer.h"
#include "../Monster.h"
#include "../../Chunk.h"
#include "../../World.h"

cBehaviorWanderer::cBehaviorWanderer() : m_IdleInterval(0)
{

}





void cBehaviorWanderer::AttachToMonster(cMonster & a_Parent)
{
	LOGD("mobDebug - Behavior Wanderer: Attach");
	m_Parent = &a_Parent;
	m_Parent->AttachTickBehavior(this);
}





bool cBehaviorWanderer::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	// Wandering behavior always happily accepts control.
	// It should therefore be the last one attached to a monster.
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	return true;
}



bool cBehaviorWanderer::ControlStarting(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	m_Parent->GetPathFinder().setDontCare(true); // We don't care we're we are going when
	// wandering. If a path is not found, the pathfinder just modifies our destination.
	return true;
}


bool cBehaviorWanderer::ControlEnding(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	m_Parent->GetPathFinder().setDontCare(false);
	return true;
}


void cBehaviorWanderer::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_Parent->IsPathFinderActivated())
	{
		return;  // Still getting there
	}

	m_IdleInterval += a_Dt;

	if (m_IdleInterval > std::chrono::seconds(1))
	{
		// At this interval the results are predictable
		int rem = m_Parent->GetWorld()->GetTickRandomNumber(6) + 1;
		m_IdleInterval -= std::chrono::seconds(1);  // So nothing gets dropped when the server hangs for a few seconds

		Vector3d Dist;
		Dist.x = static_cast<double>(m_Parent->GetWorld()->GetTickRandomNumber(10)) - 5.0;
		Dist.z = static_cast<double>(m_Parent->GetWorld()->GetTickRandomNumber(10)) - 5.0;

		if ((Dist.SqrLength() > 2)  && (rem >= 3))
		{

			Vector3d Destination(m_Parent->GetPosX() + Dist.x, m_Parent->GetPosition().y, m_Parent->GetPosZ() + Dist.z);

			cChunk * Chunk = a_Chunk.GetNeighborChunk(static_cast<int>(Destination.x), static_cast<int>(Destination.z));
			if ((Chunk == nullptr) || !Chunk->IsValid())
			{
				return;
			}

			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			int RelX = static_cast<int>(Destination.x) - Chunk->GetPosX() * cChunkDef::Width;
			int RelZ = static_cast<int>(Destination.z) - Chunk->GetPosZ() * cChunkDef::Width;
			int YBelowUs = static_cast<int>(Destination.y) - 1;
			if (YBelowUs >= 0)
			{
				Chunk->GetBlockTypeMeta(RelX, YBelowUs, RelZ, BlockType, BlockMeta);
				if (BlockType != E_BLOCK_STATIONARY_WATER)  // Idle mobs shouldn't enter water on purpose
				{
					m_Parent->MoveToPosition(Destination);
				}
			}
		}
	}
}
