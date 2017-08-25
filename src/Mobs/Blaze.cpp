
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"
#include "../World.h"
#include "../Entities/FireChargeEntity.h"




cBlaze::cBlaze(void) :
	super("Blaze", mtBlaze, "entity.blaze.hurt", "entity.blaze.death", 0.6, 1.8)
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
	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0))
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;

		auto FireCharge = cpp14::make_unique<cFireChargeEntity>(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		auto FireChargePtr = FireCharge.get();
		if (!FireChargePtr->Initialize(std::move(FireCharge), *m_World))
		{
			return false;
		}

		ResetAttackCooldown();
		// ToDo: Shoot 3 fireballs instead of 1.

		return true;
	}
	return false;
}
