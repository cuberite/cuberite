
#pragma once

#include "Monster.h"





class cAggressiveMonster:
	public cMonster
{
	using Super = cMonster;

public:

	cAggressiveMonster(
		const AString & a_ConfigName,
		eMonsterType a_MobType,
		const AString & a_SoundHurt,
		const AString & a_SoundDeath,
		const AString & a_SoundAmbient,
		float a_Width,
		float a_Height
	);

	virtual void Tick          (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void InStateChasing(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void EventSeeTarget(cPawn * a_Pawn, cChunk & a_Chunk) override;

	/** Try to perform attack
	returns true if attack was deemed successful (hit player, fired projectile, creeper exploded, etc.) even if it didn't actually do damage
	return false if e.g. the mob is still in cooldown from a previous attack */
	virtual bool Attack(std::chrono::milliseconds a_Dt);

	/** Checks if this mob sees a suitable monster - can be used e.g. to make hostile mobs attack an iron golem.
	Suitability will be determined by calling CanBeTarget */
	void CheckEventSeeMob(cChunk & a_Chunk);
	void EventSeeMob(cMonster * a_Monster, cChunk & a_Chunk) { EventSeeTarget(a_Monster, a_Chunk); }

	/** Checks if the targeted mob is out of sight */
	void CheckEventLostMob(std::chrono::milliseconds a_Dt) { CheckEventLostPlayer(a_Dt); }
	void EventLoseMob(void) { EventLosePlayer(); }
} ;




