
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chunk.h"
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
			m_EMState = CHASING;
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




