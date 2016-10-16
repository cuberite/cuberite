
#pragma once

#include "AggressiveMonster.h"





class cPassiveAggressiveMonster :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cPassiveAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void EventSeePlayer(cPlayer *, cChunk & a_Chunk) override;
} ;




