#pragma once
#include "AIComponent.h"

class cAIAgressiveComponent : public cAIComponent {
protected:
	cEntity * m_Target;
public:
	cAIAgressiveComponent(cMonster * a_Monster) : cAIComponent(a_Monster), m_Target(null){}

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void InStateChasing(float a_Dt);
	
	virtual void EventSeePlayer(cEntity *);
	virtual void Attack(float a_Dt);

protected:
	bool IsMovingToTargetPosition();
}