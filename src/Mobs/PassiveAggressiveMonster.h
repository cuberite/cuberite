
#pragma once

#include "AggressiveMonster.h"





class cPassiveAggressiveMonster:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cPassiveAggressiveMonster(
		const AString & a_ConfigName,
		eMonsterType a_MobType,
		const AString & a_SoundHurt,
		const AString & a_SoundDeath,
		const AString & a_SoundAmbient,
		float a_Width,
		float a_Height
	);

	virtual void CheckEventSeePlayer(cChunk & a_Chunk) override;
} ;




