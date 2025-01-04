
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "AggressiveMonster.h"

#include "LineBlockTracer.h"
#include "World.h"
#include "Entities/Player.h"





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





cMonster * cAggressiveMonster::GetMonsterOfTypeInSight(eMonsterType a_MobType, unsigned int a_SightDistance)
{

	cMonster * FoundTarget = nullptr;
	auto MinimumDistance = static_cast<double>(a_SightDistance * a_SightDistance);

	class cCallback : public cBlockTracer::cCallbacks
	{
		public:
		bool OnNextBlock(Vector3i a_BlockPos, BlockState a_Block, eBlockFace a_EntryFace) override
		{
			return a_Block != BlockType::Air;
		}
	};

	auto Callbacks = cCallback();

	cEntityCallback Callback = [&](cEntity & a_Entity)
	{
		if (!a_Entity.IsMob())
		{
			return false;
		}

		auto & Other = dynamic_cast<cMonster &>(a_Entity);
		if (Other.GetMobType() != a_MobType)
		{
			return false;
		}

		Vector3d MyHeadPosition = GetPosition().addedY(GetHeight());
		Vector3d TargetPosition = Other.GetPosition().addedY(Other.GetHeight());
		double TargetDistance = (MyHeadPosition - TargetPosition).SqrLength();

		if (
			(MinimumDistance > TargetDistance) &&
			(TargetDistance < (a_SightDistance * a_SightDistance))
		)
		{
			FoundTarget = & Other;
			return true;
		}
		return false;
	};

	cBoundingBox CheckZone(GetPosition().addedXZ(-a_SightDistance, -a_SightDistance), GetPosition().addedXZ(a_SightDistance, a_SightDistance));
	m_World->ForEachEntityInBox(CheckZone, Callback);
	return FoundTarget;
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
			(IsNetherNative() ? cLineBlockTracer::losAirWaterLava : cLineBlockTracer::losAirWater)
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
