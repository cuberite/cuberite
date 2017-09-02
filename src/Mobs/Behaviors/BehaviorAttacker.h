#pragma once

class cBehaviorAttacker;

#include "Behavior.h"


/** Grants attack capability to the mob. Note that this is not the same as agression!
The mob may possess this trait and not attack anyone or only attack when provoked.
Unlike most traits, this one has several forms, and therefore it is an abstract type
You should use one of its derived classes, and you cannot use it directly. */
class cBehaviorAttacker : public cBehavior
{

public:
	cBehaviorAttacker();
	void AttachToMonster(cMonster & a_Parent);


	// Our host monster will call these once it loads its config file
	void SetAttackRate(float a_AttackRate);
	void SetAttackRange(int a_AttackRange);
	void SetAttackDamage(int a_AttackDamage);

	// Behavior functions
	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Destroyed() override;
	void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

	/** Returns the target pointer, or a nullptr if we're not targeting anyone. */
	cPawn * GetTarget();

	/** Sets a new target. Forgets the older target if present. Set this to nullptr to unset target. */
	void SetTarget(cPawn * a_Target);

	/** Makes the mob strike a target the next tick. Ignores the strike cooldown.
	 * Ignored if already striking or if no target is set. */
	void StrikeTarget();

	/** Makes the mob strike a target the next tick only if the strike cooldown permits it.
	 * Ignored if already striking or if no target is set. */
	void StrikeTargetIfReady();
protected:

	/** Called when the actual attack should be made. Will be called again and again every tick until
	it returns false. a_StrikeTickCnt tracks how many times it was called. It is 1 the first call.
	It increments by 1 each call. This mechanism allows multi-tick attacks, like blazes shooting multiple
	fireballs, but most attacks are single tick and return true the first call. */
	virtual bool DoStrikeTarget(int a_StrikeTickCnt) = 0;

	// Target related methods
	bool TargetIsInStrikeRadius();
	bool TargetIsInLineOfSight();
	bool TargetTooFar();
	void StrikeTargetIfReady(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);

	// Cooldown stuff
	void ResetStrikeCooldown();

	// Our attacking parameters (Set by the setter methods, loaded from a config file in cMonster)
	float m_AttackRate;
	int m_AttackDamage;
	int m_AttackRange;
	int m_AttackCoolDownTicksLeft;

	bool m_IsStriking;

	/** Our parent */
	cMonster * m_Parent;

private:

	// The mob we want to attack
	cPawn * m_Target;
	int m_StrikeTickCnt;

};
