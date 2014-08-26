#pragma once
#include "AIComponent.h"

class cEntity;

class cAIAggressiveComponent : public cAIComponent {
	typedef cAIComponent super;
protected:
public:
	cAIAggressiveComponent(cMonster * a_Monster);
	virtual void Tick(float a_Dt, cChunk & a_Chunk) /*override*/;
};
