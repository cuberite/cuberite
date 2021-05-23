
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "AggressiveMonster.h"

#include "../World.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"





cAggressiveMonster::cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, const AString & a_SoundAmbient, float a_Width, float a_Height) :
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

	// Set or clear m_Target depending on rules for this Monster:
	if (m_EMState == CHASING)
	{
		CheckEventLostPlayer(a_Dt);
	}
	else
	{
		CheckEventSeePlayer(a_Chunk);
	}

	if (
		(GetTarget() != nullptr) &&
		TargetIsInRange() &&
		cLineBlockTracer::LineOfSightTrace(
			*GetWorld(),
			GetPosition().addedY(GetHeight()),
			GetTarget()->GetPosition().addedY(GetTarget()->GetHeight()),
			cLineBlockTracer::losAirWaterLava  // TODO: Currently all mobs see through lava, but only Nether-native mobs should be able to.
		) &&
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
