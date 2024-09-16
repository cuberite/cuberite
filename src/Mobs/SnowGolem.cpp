
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chunk.h"
#include "SnowGolem.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Entities/ThrownSnowballEntity.h"





cSnowGolem::cSnowGolem(void) :
	Super("SnowGolem", mtSnowGolem, "entity.snowman.hurt", "entity.snowman.death", "entity.snowman.ambient", 0.7f, 1.9f)
{
}





void cSnowGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	AddRandomDropItem(a_Drops, 0, 15, Item::Snowball);
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
		if (IsBlockAir(Chunk->GetBlock(Rel)) && cBlockInfo::IsSolid(Chunk->GetBlock(Below)))
		{
			Chunk->SetBlock(Rel, Block::Snow::Snow(1));
		}
	}
}





bool cSnowGolem::Attack(std::chrono::milliseconds a_Dt)
{
	UNUSED(a_Dt);

	// Comment inherited from skeletons
	StopMovingToPosition();  // Todo handle this in a better way, the snowman does some uneeded recalcs due to inStateChasing

	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0))
	{
		auto & Random = GetRandomProvider();
		Vector3d Inaccuracy = Vector3d(Random.RandReal<double>(-0.75, 0.75), Random.RandReal<double>(-0.75, 0.75), Random.RandReal<double>(-0.75, 0.75));

		// The projectile is launched from the head
		const auto HeadPos = GetPosition().addedY(1.5);
		// It aims around the head / chest
		const auto TargetPos = GetTarget()->GetPosition().addedY(GetTarget()->GetHeight() * 0.75);
		// With this data, we can calculate the speed
		const auto Speed = (TargetPos + Inaccuracy - HeadPos) * 5;

		auto Snowball = std::make_unique<cThrownSnowballEntity>(this, HeadPos, Speed);
		auto SnowballPtr = Snowball.get();
		if (!SnowballPtr->Initialize(std::move(Snowball), *GetWorld()))
		{
			return false;
		}

		ResetAttackCooldown();
		return true;
	}
	return false;
}
