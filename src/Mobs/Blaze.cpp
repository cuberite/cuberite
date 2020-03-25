
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"
#include "../World.h"
#include "../Entities/FireChargeEntity.h"




cBlaze::cBlaze(void) :
	super("Blaze", mtBlaze, "entity.blaze.hurt", "entity.blaze.death", "entity.blaze.ambient", 0.6, 1.8),
	m_bIsCharging(false),
	m_ChargeTimer(0)
{
	SetGravity(-8.0f);
	SetAirDrag(0.05f);
}





void cBlaze::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if ((a_Killer != nullptr) && (a_Killer->IsPlayer() || a_Killer->IsA("cWolf")))
	{
		unsigned int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
		AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_BLAZE_ROD);
	}
}





bool cBlaze::Attack(std::chrono::milliseconds a_Dt)
{
	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0) && (!m_bIsCharging))
	{
		m_bIsCharging = true;
		return true;
	}
	return false;
}






void cBlaze::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_bIsCharging)
	{
		m_ChargeTimer++;
		if (
			(m_ChargeTimer == 5) ||
			(m_ChargeTimer == 10) ||
			(m_ChargeTimer == 15)
		)
		{
			Vector3d Speed = GetLookVector() * 20;
			Speed.y = Speed.y + 1;

			auto FireCharge = cpp14::make_unique<cFireChargeEntity>(this, GetPosition().addedY(1), Speed);
			auto FireChargePtr = FireCharge.get();
			FireChargePtr->Initialize(std::move(FireCharge), *m_World);

			m_World->BroadcastSoundEffect("entity.ghast.shoot", GetPosition(), 4.0f, 1.0f);
		}
	}
	
	if ((m_bIsCharging) && (m_ChargeTimer > 15))
	{
		m_ChargeTimer = 0;
		m_bIsCharging = false;
		ResetAttackCooldown();
	}
}



