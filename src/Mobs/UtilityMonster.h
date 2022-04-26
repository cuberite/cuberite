
#pragma once

#include "PassiveAggressiveMonster.h"





class cUtilityMonster:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cUtilityMonster(
		const AString & a_ConfigName,
		eMonsterType a_MobType,
		const AString & a_SoundHurt,
		const AString & a_SoundDeath,
		const AString & a_SoundAmbient,
		float a_Width,
		float a_Height
	);

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
} ;




