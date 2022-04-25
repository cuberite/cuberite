
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

	/** Checks if the snowman sees any hostile mobs */
	void CheckEventSeeHostile(void);
	/** Checks if the current target is out of sight */
	void CheckEventLoseHostile(std::chrono::milliseconds a_Dt);

	/** Should be called when the snowman sees a hostile mob */
	void EventSeeHostile(cPawn * a_SeenHostile);
	/** Should be called when the snowman stops targeting a hostile mob */
	void EventLoseHostile(void);
} ;




