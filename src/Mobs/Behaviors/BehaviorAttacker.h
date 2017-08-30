#pragma once

class cBehaviorAttacker;

#include "Behavior.h"
class cBehaviorStriker;


/** Grants attack capability to the mob. Note that this is not the same as agression!
The mob may possess this trait and not attack anyone or only attack when provoked.
Unlike most traits, this one has several forms, and therefore it is an abstract type
You should use one of its derived classes, and you cannot use it directly. */
class cBehaviorAttacker : public cBehavior
{

public:
	cBehaviorAttacker();
	void AttachToMonster(cMonster & a_Parent, cBehaviorStriker & a_ParentStriker);


	// Our host monster will call these once it loads its config file
	void SetAttackRate(float a_AttackRate);
	void SetAttackRange(int a_AttackRange);
	void SetAttackDamage(int a_AttackDamage);

	// Behavior functions
	virtual bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Destroyed() override;
	virtual void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

	/** Returns the target pointer, or a nullptr if we're not targeting anyone. */
	cPawn * GetTarget();

	/** Sets a new target. Forgets the older target if present. */
	void SetTarget(cPawn * a_Target);
protected:
	virtual void StrikeTarget() = 0;

	// Target related methods
	bool TargetIsInStrikeRadius();
	bool TargetIsInStrikeRadiusAndLineOfSight();
	bool TargetOutOfSight();
	void StrikeTargetIfReady();

	// Cooldown stuff
	void ResetStrikeCooldown();

	// Our attacking parameters (Set by the setter methods, loaded from a config file in cMonster)
	float m_AttackRate;
	int m_AttackDamage;
	int m_AttackRange;
	int m_AttackCoolDownTicksLeft;

	int m_TicksSinceLastDamaged;  // How many ticks ago were we last damaged by a player?

	bool m_IsStriking;
private:

	/** Our parent */
	cMonster * m_Parent;

	// The mob we want to attack
	cPawn * m_Target;

};
