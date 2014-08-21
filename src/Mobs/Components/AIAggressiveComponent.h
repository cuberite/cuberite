#pragma once
#include "AIComponent.h"

class cAIAggressiveComponent : public cAIComponent {
	typedef cAIComponent super;
protected:
	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	cEntity * m_Target;
public:
	cAIAggressiveComponent(cMonster * a_Monster);

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
	virtual void Attack(float a_Dt);
};
