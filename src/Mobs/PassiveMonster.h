
#pragma once

#include "Monster.h"
#include "../UUID.h"





class cPassiveMonster:
	public cMonster
{
	using Super = cMonster;

public:

	cPassiveMonster(
		const AString & a_ConfigName,
		eMonsterType a_MobType,
		const AString & a_SoundHurt,
		const AString & a_SoundDeath,
		const AString & a_SoundAmbient,
		double a_Width,
		double a_Height
	);

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	/** When hit by someone, run away */
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void Destroyed(void) override;
};




