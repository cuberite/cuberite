#pragma once
#include "AIComponent.h"

class cEntity;

class cAIPassiveAggressiveComponent : public cAIComponent {
	typedef cAIComponent super;
protected:
public:
	cAIPassiveAggressiveComponent(cMonster * a_Monster);
	virtual void Tick(float a_Dt, cChunk & a_Chunk) /*override*/;
};
