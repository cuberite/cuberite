#pragma once
#include "AIComponent.h"

class cEntity;

class cAIAggressiveComponent : public cAIComponent {
	typedef cAIComponent super;
	void EventSeePlayer(cEntity * a_Entity);
	void EventLosePlayer(void);
protected:
	void InStateChasing(float a_Dt);

	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	enum MPersonality{PASSIVE, AGGRESSIVE, COWARDLY} m_EMPersonality;

	cEntity * m_Target;
	/** Coordinates of the next position that should be reached */
	Vector3d m_Destination;
	/** Coordinates for the ultimate, final destination. */
	Vector3d m_FinalDestination;
	/** A semi-temporary list to store the traversed coordinates during active pathfinding so we don't visit them again */
	std::vector<Vector3i> m_TraversedCoordinates;

	/** Stores if mobile is currently moving towards the ultimate, final destination */
	bool m_bMovingToDestination;

	inline void FinishPathFinding(void)
	{
		m_TraversedCoordinates.clear();
		m_bMovingToDestination = false;
	}
	/** Finds the next place to go
		This is based on the ultimate, final destination and the current position, as well as the traversed coordinates, and any environmental hazards */
	void TickPathFinding(void);

	inline bool IsCoordinateInTraversedList(Vector3i a_Coords)
	{
		return (std::find(m_TraversedCoordinates.begin(), m_TraversedCoordinates.end(), a_Coords) != m_TraversedCoordinates.end());
	}
	bool IsMovingToTargetPosition();
	bool ReachedFinalDestination();
	void CheckEventSeePlayer(void);
	void CheckEventLostPlayer(void);
public:
	cAIAggressiveComponent(cMonster * a_Monster);

	virtual void Tick(float a_Dt, cChunk & a_Chunk) /*override*/;
	
	virtual void Attack(float a_Dt);
	void MoveToPosition(const Vector3d & a_Position);
};
