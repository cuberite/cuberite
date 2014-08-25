
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "AIAggressiveComponent.h"
#include <iostream>

#include "../Monster.h"

#include "../../World.h"
#include "../../Entities/Player.h"
#include "../../Tracer.h"





cAIAggressiveComponent::cAIAggressiveComponent(cMonster * a_Monster) : cAIComponent(a_Monster), m_Target(NULL){
	m_EMPersonality = AGGRESSIVE;
}





void cAIAggressiveComponent::Tick(float a_Dt, cChunk & a_Chunk)
{
	if (m_EMState == CHASING)
	{
		CheckEventLostPlayer();
	}
	else
	{
		CheckEventSeePlayer();
	}

	if (m_Target == NULL)
		return;

	cTracer LineOfSight(m_Self->GetWorld());
	Vector3d AttackDirection(m_Target->GetPosition() - m_Self->GetPosition());

	if (ReachedFinalDestination() && !LineOfSight.Trace(m_Self->GetPosition(), AttackDirection, (int)AttackDirection.Length()))
	{
		// Attack if reached destination, target isn't null, and have a clear line of sight to target (so won't attack through walls)
		//Attack(a_Dt / 1000);
	}
}





void cAIAggressiveComponent::Attack(float a_Dt)
{
}





void cAIAggressiveComponent::EventSeePlayer(cEntity * a_Entity)
{
	if (!((cPlayer *)a_Entity)->IsGameModeCreative())
	{
		m_Target = a_Entity;
		m_EMState = CHASING;
	}
}




// What to do if in Chasing State
void cAIAggressiveComponent::InStateChasing(float a_Dt)
{
	if (m_Target != NULL)
	{
		if (m_Target->IsPlayer())
		{
			if (((cPlayer *)m_Target)->IsGameModeCreative())
			{
				m_EMState = IDLE;
				return;
			}
		}

		if (!IsMovingToTargetPosition())
		{
			MoveToPosition(m_Target->GetPosition());
		}
	}
}




bool cAIAggressiveComponent::ReachedFinalDestination()
{
	if ((m_Self->GetPosition() - m_FinalDestination).Length() <= m_Self->GetAttackComponent().GetAttackRange())
	{
		return true;
	}
	
	return false;
}





void cAIAggressiveComponent::MoveToPosition(const Vector3d & a_Position)
{
	FinishPathFinding();

	m_FinalDestination = a_Position;
	m_bMovingToDestination = true;
	TickPathFinding();
}





void cAIAggressiveComponent::TickPathFinding()
{
	const int PosX = (int)floor(m_Self->GetPosX());
	const int PosY = (int)floor(m_Self->GetPosY());
	const int PosZ = (int)floor(m_Self->GetPosZ());

	std::vector<Vector3d> m_PotentialCoordinates;
	m_TraversedCoordinates.push_back(Vector3i(PosX, PosY, PosZ));

	static const struct  // Define which directions to try to move to
	{
		int x, z;
	} gCrossCoords[] =
	{
		{ 1, 0},
		{-1, 0},
		{ 0, 1},
		{ 0, -1},
	} ;
	
	if ((PosY - 1 < 0) || (PosY + 2 > cChunkDef::Height) /* PosY + 1 will never be true if PosY + 2 is not */)
	{
		// Too low/high, can't really do anything
		FinishPathFinding();
		return;
	}

	for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
	{
		if (IsCoordinateInTraversedList(Vector3i(gCrossCoords[i].x + PosX, PosY, gCrossCoords[i].z + PosZ)))
		{
			continue;
		}

		BLOCKTYPE BlockAtY = m_Self->GetWorld()->GetBlock(gCrossCoords[i].x + PosX, PosY, gCrossCoords[i].z + PosZ);
		BLOCKTYPE BlockAtYP = m_Self->GetWorld()->GetBlock(gCrossCoords[i].x + PosX, PosY + 1, gCrossCoords[i].z + PosZ);
		BLOCKTYPE BlockAtYPP = m_Self->GetWorld()->GetBlock(gCrossCoords[i].x + PosX, PosY + 2, gCrossCoords[i].z + PosZ);
		int LowestY = m_Self->GetMovementComponent().FindFirstNonAirBlockPosition(gCrossCoords[i].x + PosX, gCrossCoords[i].z + PosZ);
		BLOCKTYPE BlockAtLowestY = m_Self->GetWorld()->GetBlock(gCrossCoords[i].x + PosX, LowestY, gCrossCoords[i].z + PosZ);

		if (
			(!cBlockInfo::IsSolid(BlockAtY)) &&
			(!cBlockInfo::IsSolid(BlockAtYP)) &&
			(!IsBlockLava(BlockAtLowestY)) &&
			(BlockAtLowestY != E_BLOCK_CACTUS) &&
			(PosY - LowestY < 4)
			)
		{
			m_PotentialCoordinates.push_back(Vector3d((gCrossCoords[i].x + PosX), PosY, gCrossCoords[i].z + PosZ));
		}
		else if (
			(cBlockInfo::IsSolid(BlockAtY)) &&
			(BlockAtY != E_BLOCK_CACTUS) &&
			(!cBlockInfo::IsSolid(BlockAtYP)) &&
			(!cBlockInfo::IsSolid(BlockAtYPP)) &&
			(BlockAtY != E_BLOCK_FENCE) &&
			(BlockAtY != E_BLOCK_FENCE_GATE)
			)
		{
			m_PotentialCoordinates.push_back(Vector3d((gCrossCoords[i].x + PosX), PosY + 1, gCrossCoords[i].z + PosZ));
		}
	}

	if (!m_PotentialCoordinates.empty())
	{
		Vector3f ShortestCoords = m_PotentialCoordinates.front();
		for (std::vector<Vector3d>::const_iterator itr = m_PotentialCoordinates.begin(); itr != m_PotentialCoordinates.end(); ++itr)
		{
			Vector3f Distance = m_FinalDestination - ShortestCoords;
			Vector3f Distance2 = m_FinalDestination - *itr;
			if (Distance.SqrLength() > Distance2.SqrLength())
			{
				ShortestCoords = *itr;
			}
		}

		m_Destination = ShortestCoords;
		m_Destination.z += 0.5f;
		m_Destination.x += 0.5f;
	}
	else
	{
		FinishPathFinding();
	}
}





bool cAIAggressiveComponent::IsMovingToTargetPosition()
{
	// Difference between destination x and target x is negligible (to 10^-12 precision)
	if (fabsf((float)m_FinalDestination.x - (float)m_Target->GetPosX()) < std::numeric_limits<float>::epsilon())
	{
		return false;
	}
	// Difference between destination z and target z is negligible (to 10^-12 precision)
	else if (fabsf((float)m_FinalDestination.z - (float)m_Target->GetPosZ()) > std::numeric_limits<float>::epsilon())
	{
		return false;
	}
	return true;
}





// Checks to see if EventSeePlayer should be fired
// monster sez: Do I see the player
void cAIAggressiveComponent::CheckEventSeePlayer(void)
{
	// TODO: Rewrite this to use cWorld's DoWithPlayers()
	cPlayer * Closest = m_Self->GetWorld()->FindClosestPlayer(m_Self->GetPosition(), (float)m_Self->GetEnvironmentComponent().GetSightDistance(), false);

	if (Closest != NULL)
	{
		EventSeePlayer(Closest);
	}
}





void cAIAggressiveComponent::CheckEventLostPlayer(void)
{
	if (m_Target != NULL)
	{
		if ((m_Target->GetPosition() - m_Self->GetPosition()).Length() > m_Self->GetEnvironmentComponent().GetSightDistance())
		{
			EventLosePlayer();
		}
	}
	else
	{
		EventLosePlayer();
	}
}





void cAIAggressiveComponent::EventLosePlayer(void)
{
	m_Target = NULL;
	m_EMState = IDLE;
}
