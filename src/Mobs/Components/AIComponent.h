#pragma once

class cMonster;
class cEntity;
class cChunk;

class cAIComponent
{
protected:
	cMonster * m_Self;
	cEntity * m_Target;
	float m_IdleInterval;

	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;

	/** Coordinates of the next position that should be reached */
	Vector3d m_Destination;
	/** Coordinates for the ultimate, final destination. */
	Vector3d m_FinalDestination;
	/** A semi-temporary list to store the traversed coordinates during active pathfinding so we don't visit them again */
	std::vector<Vector3i> m_TraversedCoordinates;

	/** Stores if mobile is currently moving towards the ultimate, final destination */
	bool m_bMovingToDestination;

	/**********
	* Pathfinding
	**********/
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
	virtual bool ReachedDestination(void);
	virtual void MoveToPosition(const Vector3d & a_Position);
	void SetPitchAndYawFromDestination();

	/**********
	* Event Management
	**********/
	// void EventLosePlayer(void);
	// virtual void EventSeePlayer(cEntity * a_Entity);
	virtual void InStateIdle    (float a_Dt);
	virtual void InStateChasing (float a_Dt);
	virtual void InStateEscaping(float a_Dt);
public:
	cAIComponent(cMonster * a_Entity);
	virtual ~cAIComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk);
};
