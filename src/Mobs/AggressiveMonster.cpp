
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "AggressiveMonster.h"

#include "../World.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"





cAggressiveMonster::cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, const AString & a_SoundAmbient, double a_Width, double a_Height) :
	Super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_SoundAmbient, a_Width, a_Height)
{
	m_EMPersonality = AGGRESSIVE;
}





// What to do if in Chasing State
void cAggressiveMonster::InStateChasing(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::InStateChasing(a_Dt, a_Chunk);

	if (GetTarget() != nullptr)
	{
		MoveToPosition(GetTarget()->GetPosition());
	}
}





void cAggressiveMonster::EventSeePlayer(cPlayer * a_Player, cChunk & a_Chunk)
{
	if (!a_Player->CanMobsTarget())
	{
		return;
	}

	Super::EventSeePlayer(a_Player, a_Chunk);
	m_EMState = CHASING;
}





void cAggressiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
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

	auto target = GetTarget();
	if (target == nullptr)
	{
		return;
	}

	// TODO: Currently all mobs see through lava, but only Nether-native mobs should be able to.
	Vector3d MyHeadPosition = GetPosition() + Vector3d(0, GetHeight(), 0);
	Vector3d TargetPosition = target->GetPosition() + Vector3d(0, target->GetHeight(), 0);
	if (
		TargetIsInRange() &&
		cLineBlockTracer::LineOfSightTrace(*GetWorld(), MyHeadPosition, TargetPosition, cLineBlockTracer::losAirWaterLava) &&
		(GetHealth() > 0.0)
	)
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

	double KnockbackAmount = 9;
	GetTarget()->TakeDamage(dtMobAttack, this, m_AttackDamage, KnockbackAmount);

	return true;
}
