
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ghast.h"
#include "../World.h"
#include "../Entities/GhastFireballEntity.h"




cGhast::cGhast(void) :
	super("Ghast", mtGhast, "entity.ghast.hurt", "entity.ghast.death", "entity.ghast.ambient", 4, 4),
	m_bIsCharging(false),
	m_ChargeTimer(0)
{
	SetGravity(0);
	SetAirDrag(0.1f);
}





void cGhast::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GHAST_TEAR);
}





bool cGhast::Attack(std::chrono::milliseconds a_Dt)
{
	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0) && (!m_bIsCharging))
	{
		auto & Random = GetRandomProvider();
		auto SoundPitchMultiplier = 1.0f + (Random.RandReal(1.0f) - Random.RandReal(1.0f)) * 0.2f;

		m_World->BroadcastSoundEffect("entity.ghast.warn", GetPosition(), 4.0f, SoundPitchMultiplier * 0.9f);
		m_bIsCharging = true;
		m_World->BroadcastEntityMetadata(*this);
		return true;
	}
	return false;
}





void cGhast::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if ((m_bIsCharging) && (m_ChargeTimer++ == 10))
	{
		m_ChargeTimer = 0;
		m_bIsCharging = false;
		m_World->BroadcastEntityMetadata(*this);

		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;

		auto GhastBall = cpp14::make_unique<cGhastFireballEntity>(this, GetPosition(), Speed);
		auto GhastBallPtr = GhastBall.get();
		GhastBallPtr->Initialize(std::move(GhastBall), *m_World);

		m_World->BroadcastSoundEffect("entity.ghast.shoot", GetPosition(), 4.0f, 1.0f);

		ResetAttackCooldown();
	}

	// Primitive way to fly
	if (GetPosY() < GetRandomProvider().RandInt(60, 80)) {
		AddSpeedY(1);
	} else {
		AddSpeedY(-1);
	}
}



