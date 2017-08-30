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

	// Functions our host Monster should invoke:
	bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void Destroyed() override;
	void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

	// Our host monster will call these once it loads its config file
	void SetAttackRate(float a_AttackRate);
	void SetAttackRange(int a_AttackRange);
	void SetAttackDamage(int a_AttackDamage);

	/** Returns the target pointer, or a nullptr if we're not targeting anyone. */
	cPawn * GetTarget();

	/** Sets the target. */
	void SetTarget(cPawn * a_Target);
protected:
	void ApproachTarget();
	// virtual void ApproachTarget() = 0; //mobTodo
private:

	/** Our parent */
	cMonster * m_Parent;
	cBehaviorStriker * m_ParentStriker;

	// The mob we want to attack
	cPawn * m_Target;

	// Target stuff
	bool TargetIsInStrikeRange();
	bool TargetOutOfSight();
	void StrikeTarget();

	// Cooldown stuff
	void ResetStrikeCooldown();

	// Our attacking parameters (Set by the setter methods, loaded from a config file in cMonster)
	float m_AttackRate;
	int m_AttackDamage;
	int m_AttackRange;
	int m_AttackCoolDownTicksLeft;

	int m_TicksSinceLastDamaged;  // How many ticks ago were we last damaged by a player?
};
