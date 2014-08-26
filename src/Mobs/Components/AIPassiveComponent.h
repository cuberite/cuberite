#pragma once
#include "AIComponent.h"

class cEntity;

class cAIPassiveComponent : public cAIComponent {
	typedef cAIComponent super;
protected:
public:
	cAIPassiveComponent(cMonster * a_Monster);
	virtual void Tick(float a_Dt, cChunk & a_Chunk) /*override*/;
};
