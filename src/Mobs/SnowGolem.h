
#pragma once

#include "PassiveAggressiveMonster.h"





class cSnowGolem:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cSnowGolem();

	CLASS_PROTODEF(cSnowGolem)

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;

	void CheckEventSeeHostile(void);
	void CheckEventLoseHostile(std::chrono::milliseconds a_Dt);

	void EventSeeHostile(cPawn * a_SeenHostile);
	void EventLoseHostile(void);
} ;




