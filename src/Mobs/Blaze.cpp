
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"
#include "../World.h"
#include "../Entities/FireChargeEntity.h"




cBlaze::cBlaze(void) :
	super("Blaze", mtBlaze, "mob.blaze.hit", "mob.blaze.death", 0.6, 1.8)
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
	m_AttackInterval += (static_cast<float>(a_Dt.count()) / 1000) * m_AttackRate;

	if ((m_Target != nullptr) && (m_AttackInterval > 3.0))
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;
		cFireChargeEntity * FireCharge = new cFireChargeEntity(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		if (FireCharge == nullptr)
		{
			return false;
		}
		if (!FireCharge->Initialize(*m_World))
		{
			delete FireCharge;
			FireCharge = nullptr;
			return false;
		}
		m_World->BroadcastSpawnEntity(*FireCharge);
		m_AttackInterval = 0.0;
		// ToDo: Shoot 3 fireballs instead of 1.
		return true;
	}
	return false;
}
