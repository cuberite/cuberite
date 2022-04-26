
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chunk.h"
#include "LineBlockTracer.h"
#include "SnowGolem.h"
#include "../BlockInfo.h"
#include "../Entities/ThrownSnowballEntity.h"
#include "../World.h"





cSnowGolem::cSnowGolem(void) :
	Super("SnowGolem", mtSnowGolem, "entity.snowman.hurt", "entity.snowman.death", "entity.snowman.ambient", 0.7f, 1.9f)
{
}





bool cSnowGolem::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	// Make sure that snow golems will never target players and other snow golems
	auto OldTarget = GetTarget();
	auto OldState = m_EMState;

	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	// Quick and dirty fix: just revert changes made by Super::DoTakeDamage...
	// unless the new target is a hostile and it isn't a ghast
	if (OldTarget != GetTarget())
	{
		if (
			(GetTarget() != nullptr) && (GetTarget()->IsMob()) &&
			(static_cast<cMonster *>(GetTarget())->GetMobFamily() == mfHostile) &&
			(static_cast<cMonster *>(GetTarget())->GetMobType() != mtGhast)
		)
		{
			EventSeeHostile(GetTarget());
		}
		else
		{
			SetTarget(OldTarget);
			m_EMState = OldState;
		}
	}

	return true;
}





void cSnowGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	AddRandomDropItem(a_Drops, 0, 15, E_ITEM_SNOWBALL);
}





void cSnowGolem::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	PREPARE_REL_AND_CHUNK(GetPosition().Floor(), a_Chunk);
	if (!RelSuccess)
	{
		return;
	}

	if (IsBiomeNoDownfall(Chunk->GetBiomeAt(Rel.x, Rel.z)))
	{
		TakeDamage(dtEnvironment, nullptr, GetRawDamageAgainst(*this), GetKnockbackAmountAgainst(*this));
	}
	else if (const auto Below = Rel.addedY(-1); Below.y >= 0)
	{
		if ((Chunk->GetBlock(Rel) == E_BLOCK_AIR) && cBlockInfo::IsSolid(Chunk->GetBlock(Below)))
		{
			Chunk->SetBlock(Rel, E_BLOCK_SNOW, 0);
		}
	}

	// Set or clear m_Target:
	if (m_EMState == CHASING)
	{
		CheckEventLoseHostile(a_Dt);
	}
	else
	{
		CheckEventSeeHostile();
	}
}





bool cSnowGolem::Attack(std::chrono::milliseconds a_Dt)
{
	UNUSED(a_Dt);

	// Comment inherited from cSkeleton
	StopMovingToPosition();  // Todo handle this in a better way, the snowman does some uneeded recalcs due to inStateChasing
	auto & Random = GetRandomProvider();

	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0))
	{
		Vector3d Inaccuracy = Vector3d(Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25));
		Vector3d Speed = (GetTarget()->GetPosition() + Inaccuracy - GetPosition()) * 5;
		Speed.y += Random.RandInt(-1, 1);

		auto Snowball = std::make_unique<cThrownSnowballEntity>(this, GetPosition().addedY(1.5), Speed);
		auto SnowballPtr = Snowball.get();
		if (!SnowballPtr->Initialize(std::move(Snowball), *m_World))
		{
			return false;
		}

		ResetAttackCooldown();
		return true;
	}
	return false;
}





// Checks to see if there are hostile mobs nearby
// Basically cMonster::CheckEventLostPlayer copied and adjusted
void cSnowGolem::CheckEventSeeHostile(void)
{
	if (GetTarget() != nullptr)
	{
		return;
	}

	cMonster * TargetMonster = nullptr;
	double ClosestDistance = m_SightDistance * m_SightDistance;  // Squared
	const auto MyHeadPosition = GetPosition().addedY(GetHeight());

	// Enumerate all monsters within sight:
	m_World->ForEachEntityInBox(
		cBoundingBox(MyHeadPosition, m_SightDistance * 2),
		[&](cEntity & a_Entity)
	{
		if (!a_Entity.IsMob())
		{
			return false;
		}

		auto Monster = static_cast<cMonster *>(&a_Entity);

		if (Monster->GetMobFamily() != mfHostile)
		{
			return false;
		}

		// Snow golems will never attack ghasts
		if (Monster->GetMobType() == mtGhast)
		{
			return false;
		}

		const auto TargetHeadPosition = Monster->GetPosition().addedY(Monster->GetHeight());
		const auto TargetDistance = (TargetHeadPosition - MyHeadPosition).SqrLength();

		if (
			(TargetDistance < ClosestDistance) &&
			cLineBlockTracer::LineOfSightTrace(*GetWorld(), MyHeadPosition, TargetHeadPosition, cLineBlockTracer::losAirWater)
		)
		{
			TargetMonster = Monster;
			ClosestDistance = TargetDistance;
		}

		return false;
	});

	// Target it if suitable monster found:
	if (TargetMonster != nullptr)
	{
		EventSeeHostile(TargetMonster);
	}
}





// Basically cMonster::CheckEventLostPlayer copied and adjusted
void cSnowGolem::CheckEventLoseHostile(const std::chrono::milliseconds a_Dt)
{
	const auto Target = GetTarget();

	if ((Target == nullptr) || (!Target->IsTicking()))
	{
		m_EMState = IDLE;
		return;
	}

	// Check if the target is too far away:
	if (!Target->GetBoundingBox().DoesIntersect({ GetPosition(), m_SightDistance * 2.0 }))
	{
		EventLoseHostile();
		return;
	}

	const auto MyHeadPosition = GetPosition().addedY(GetHeight());
	const auto TargetHeadPosition = Target->GetPosition().addedY(Target->GetHeight());
	if (!cLineBlockTracer::LineOfSightTrace(*GetWorld(), MyHeadPosition, TargetHeadPosition, cLineBlockTracer::losAirWater))
	{
		if ((m_LoseSightAbandonTargetTimer += a_Dt) > std::chrono::seconds(4))
		{
			EventLoseHostile();
		}
	}
	else
	{
		// Subtract the amount of time we "handled" instead of setting to zero, so we don't ignore a large a_Dt of say, 8s:
		m_LoseSightAbandonTargetTimer -= std::min(std::chrono::milliseconds(4000), m_LoseSightAbandonTargetTimer);
	}
}





void cSnowGolem::EventSeeHostile(cPawn * a_SeenHostile)
{
	SetTarget(a_SeenHostile);

	m_EMState = CHASING;
}





void cSnowGolem::EventLoseHostile(void)
{
	SetTarget(nullptr);

	m_EMState = IDLE;
	m_LoseSightAbandonTargetTimer = std::chrono::seconds::zero();
}
