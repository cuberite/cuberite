
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "AggressiveMonster.h"

#include "../World.h"
#include "../Entities/Player.h"
#include "../Tracer.h"





cAggressiveMonster::cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height)
{
	m_EMPersonality = AGGRESSIVE;
}





// What to do if in Chasing State
void cAggressiveMonster::InStateChasing(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::InStateChasing(a_Dt, a_Chunk);

	if (GetTarget() != nullptr)
	{
		MoveToPosition(GetTarget()->GetPosition());
	}
}





void cAggressiveMonster::EventSeePlayer(cEntity * a_Entity, cChunk & a_Chunk)
{
	if (!static_cast<cPlayer *>(a_Entity)->IsGameModeCreative())
	{
		super::EventSeePlayer(a_Entity, a_Chunk);
		m_EMState = CHASING;
	}
}





void cAggressiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_EMState == CHASING)
	{
		CheckEventLostPlayer();
	}
	else
	{
		CheckEventSeePlayer(a_Chunk);
	}

	if (GetTarget() == nullptr)
	{
		return;
	}

	cTracer LineOfSight(GetWorld());
	Vector3d MyHeadPosition = GetPosition() + Vector3d(0, GetHeight(), 0);
	Vector3d AttackDirection(GetTarget()->GetPosition() + Vector3d(0, GetTarget()->GetHeight(), 0) - MyHeadPosition);


	if (TargetIsInRange() && !LineOfSight.Trace(MyHeadPosition, AttackDirection, static_cast<int>(AttackDirection.Length())) && (GetHealth() > 0.0))
	{
		// Attack if reached destination, target isn't null, and have a clear line of sight to target (so won't attack through walls)
		Attack(a_Dt);
	}
}





bool cAggressiveMonster::Attack(std::chrono::milliseconds a_Dt)
{
	if ((GetTarget() == nullptr) || (m_AttackCoolDownTicksLeft != 0))
	{
		return false;
	}

	// Setting this higher gives us more wiggle room for attackrate
	ResetAttackCooldown();
	GetTarget()->TakeDamage(dtMobAttack, this, m_AttackDamage, 0);

	return true;
}
