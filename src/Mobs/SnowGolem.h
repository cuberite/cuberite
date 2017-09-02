
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorWanderer.h"




class cSnowGolem :
	public cMonster
{
	typedef cMonster super;

public:
	cSnowGolem(void);

	CLASS_PROTODEF(cSnowGolem)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

private:
	cBehaviorWanderer m_BehaviorWanderer;
} ;




