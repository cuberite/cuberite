#pragma once
#include "AIComponent.h"

class cAIAgressiveComponent : public cAIComponent {
protected:
	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	cEntity * m_Target;
public:
	cAIAgressiveComponent(cMonster * a_Monster) : cAIComponent(a_Monster), m_Target(null){}

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void InStateChasing(float a_Dt);
	
	virtual void EventSeePlayer(cEntity *);
	virtual void Attack(float a_Dt);

protected:
	virtual void EventLosePlayer(void);
	virtual void CheckEventLostPlayer(void);
	
	bool IsMovingToTargetPosition();
}