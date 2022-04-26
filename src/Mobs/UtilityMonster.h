
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

	/** Checks if the mob sees a suitable monster */
	void CheckEventSeeMob(cChunk & a_Chunk);

	/** Checks if the current target is out of sight */
	void CheckEventLoseMob(std::chrono::milliseconds a_Dt) { CheckEventLostPlayer(a_Dt); };

	/** Should be called when the utility monster sees a target */
	void EventSeeMob(cPawn * a_Pawn, cChunk & a_Chunk) { EventSeePlayer(a_Pawn, a_Chunk); };

	/** Should be called when the target is no longer in range */
	void EventLoseMob(void) { EventLosePlayer(); };
} ;




