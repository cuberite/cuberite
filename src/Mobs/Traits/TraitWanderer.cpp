#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "TraitWanderer.h"
#include "../Monster.h"
#include "../../Chunk.h"
#include "../../World.h"

cTraitWanderer::cTraitWanderer(cMonster * a_Parent)
	: m_Parent(a_Parent)
	, m_IdleInterval(0)
{
	ASSERT(m_Parent != nullptr);
}

bool cTraitWanderer::ActiveTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_Parent->IsPathFinderActivated())
	{
		return true;  // Still getting there
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
				return true;
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
	return true;
}
