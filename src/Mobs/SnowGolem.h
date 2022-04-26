
#pragma once

#include "UtilityMonster.h"





class cSnowGolem:
	public cUtilityMonster
{
	using Super = cUtilityMonster;

public:

	cSnowGolem();

	CLASS_PROTODEF(cSnowGolem)

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
} ;




