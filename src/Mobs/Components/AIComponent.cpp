#include "Globals.h"
#include "AIComponent.h"
#include "../Monster.h"

#include "../../World.h"
#include "../../Entities/Player.h"
#include "../../Tracer.h"

cAIComponent::cAIComponent(cMonster * a_Entity) : m_Self(a_Entity), m_Target(NULL), m_IdleInterval(0.0f), m_EMState(IDLE), m_bMovingToDestination(false)
{

}

void cAIComponent::Tick(float a_Dt, cChunk & a_Chunk)
{
	if ((m_Target != NULL) && m_Target->IsDestroyed())
		m_Target = NULL;


	a_Dt /= 1000;

	if (m_bMovingToDestination)
	{
		if (m_Self->GetEnvironmentComponent().GetOnGround() && m_Self->GetMovementComponent().DoesPosYRequireJump((int)floor(m_Destination.y)))
		{
			m_Self->GetEnvironmentComponent().SetOnGround(false);

			// TODO: Change to AddSpeedY once collision detection is fixed - currently, mobs will go into blocks attempting to jump without a teleport
			m_Self->AddPosY(1.2);  // Jump!!
		}

		Vector3f Distance = m_Destination - m_Self->GetPosition();
		if (!ReachedDestination() && !ReachedFinalDestination())  // If we haven't reached any sort of destination, move
		{
			Distance.y = 0;
			Distance.Normalize();

			if (m_Self->GetEnvironmentComponent().GetOnGround())
			{
				Distance *= 2.5f;
			}
			else if (m_Self->IsSwimming())
			{
				Distance *= 1.3f;
			}
			else
			{
				// Don't let the mob move too much if he's falling.
				Distance *= 0.25f;
			}

			m_Self->AddSpeedX(Distance.x);
			m_Self->AddSpeedZ(Distance.z);

			// It's too buggy!
			/*
			if (m_EMState == ESCAPING)
			{
				// Runs Faster when escaping :D otherwise they just walk away
				SetSpeedX (GetSpeedX() * 2.f);
				SetSpeedZ (GetSpeedZ() * 2.f);
			}
			*/
		}
		else
		{
			if (ReachedFinalDestination())  // If we have reached the ultimate, final destination, stop pathfinding and attack if appropriate
			{
				FinishPathFinding();
			}
			else
			{
				TickPathFinding();  // We have reached the next point in our path, calculate another point
			}
		}
	}

	SetPitchAndYawFromDestination();
	// HandleFalling();

	switch (m_EMState)
	{
		case IDLE:
		{
			// If enemy passive we ignore checks for player visibility
			InStateIdle(a_Dt);
			break;
		}
		case CHASING:
		{
			// If we do not see a player anymore skip chasing action
			InStateChasing(a_Dt);
			break;
		}
		case ESCAPING:
		{
			InStateEscaping(a_Dt);
			break;
		}
			
		case ATTACKING: break;
	}  // switch (m_EMState)

	m_Self->BroadcastMovementUpdate();
}





void cAIComponent::SetPitchAndYawFromDestination()
{
	Vector3d FinalDestination = m_FinalDestination;
	if (m_Target != NULL)
	{
		if (m_Target->IsPlayer())
		{
			FinalDestination.y = ((cPlayer *)m_Target)->GetStance();
		}
		else
		{
			FinalDestination.y = m_Self->GetHeight();
		}
	}

	Vector3d Distance = FinalDestination - m_Self->GetPosition();
	if (Distance.SqrLength() > 0.1f)
	{
		{
			double Rotation, Pitch;
			Distance.Normalize();
			VectorToEuler(Distance.x, Distance.y, Distance.z, Rotation, Pitch);
			m_Self->SetHeadYaw(Rotation);
			m_Self->SetPitch(-Pitch);
		}

		{
			Vector3d BodyDistance = m_Destination - m_Self->GetPosition();
			double Rotation, Pitch;
			Distance.Normalize();
			VectorToEuler(BodyDistance.x, BodyDistance.y, BodyDistance.z, Rotation, Pitch);
			m_Self->SetYaw(Rotation);
		}
	}
}





void cAIComponent::TickPathFinding()
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





bool cAIComponent::IsMovingToTargetPosition()
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





bool cAIComponent::ReachedFinalDestination()
{
	if ((m_Self->GetPosition() - m_FinalDestination).Length() <= m_Self->GetAttackComponent().GetAttackRange())
	{
		return true;
	}
	
	return false;
}





bool cAIComponent::ReachedDestination()
{
	if ((m_Destination - m_Self->GetPosition()).Length() < 0.5f)
	{
		return true;
	}

	return false;
}





void cAIComponent::MoveToPosition(const Vector3d & a_Position)
{
	FinishPathFinding();

	m_FinalDestination = a_Position;
	m_bMovingToDestination = true;
	TickPathFinding();
}





void cAIComponent::InStateIdle(float a_Dt)
{
	if (m_bMovingToDestination)
	{
		return;  // Still getting there
	}

	m_IdleInterval += a_Dt;

	if (m_IdleInterval > 1)
	{
		// At this interval the results are predictable
		int rem = m_Self->GetWorld()->GetTickRandomNumber(6) + 1;
		m_IdleInterval -= 1;  // So nothing gets dropped when the server hangs for a few seconds

		Vector3d Dist;
		Dist.x = (double)m_Self->GetWorld()->GetTickRandomNumber(10) - 5;
		Dist.z = (double)m_Self->GetWorld()->GetTickRandomNumber(10) - 5;

		if ((Dist.SqrLength() > 2)  && (rem >= 3))
		{
			Vector3d Destination(m_Self->GetPosX() + Dist.x, 0, m_Self->GetPosZ() + Dist.z);

			int NextHeight = m_Self->GetMovementComponent().FindFirstNonAirBlockPosition(Destination.x, Destination.z);

			if (m_Self->GetMovementComponent().IsNextYPosReachable(NextHeight))
			{
				Destination.y = NextHeight;
				MoveToPosition(Destination);
			}
		}
	}
}





// What to do if in Chasing State
// This state should always be defined in each child class
void cAIComponent::InStateChasing(float a_Dt)
{
	UNUSED(a_Dt);
}





// What to do if in Escaping State
void cAIComponent::InStateEscaping(float a_Dt)
{
	UNUSED(a_Dt);
	
	if (m_Target != NULL)
	{
		int sight_distance = m_Self->GetEnvironmentComponent().GetSightDistance();
		Vector3d newloc = m_Self->GetPosition();
		newloc.x = (m_Target->GetPosition().x < newloc.x)? (newloc.x + sight_distance): (newloc.x - sight_distance);
		newloc.z = (m_Target->GetPosition().z < newloc.z)? (newloc.z + sight_distance): (newloc.z - sight_distance);
		MoveToPosition(newloc);
	}
	else
	{
		m_EMState = IDLE;  // This shouldnt be required but just to be safe
	}
}
