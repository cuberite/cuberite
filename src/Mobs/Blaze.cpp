
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"
#include "../World.h"




cBlaze::cBlaze(void) :
	// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
	super("Blaze", mtBlaze, "mob.blaze.hit", "mob.blaze.death", 0.7, 1.8)
{
}





void cBlaze::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (a_Killer->IsA("cPlayer") || a_Killer->IsA("cWolf"))
	{
		int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(21);
		AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_BLAZE_ROD);
	}
}





void cBlaze::Attack(float a_Dt)
{
	m_AttackInterval += a_Dt * m_AttackRate;

	if (m_Target != NULL && m_AttackInterval > 3.0)
	{
		// Setting this higher gives us more wiggle room for attackrate
		Vector3d Speed = GetLookVector() * 20;
		Speed.y = Speed.y + 1;
		cFireChargeEntity * FireCharge = new cFireChargeEntity(this, GetPosX(), GetPosY() + 1, GetPosZ(), Speed);
		if (FireCharge == NULL)
		{
			return;
		}
		if (!FireCharge->Initialize(m_World))
		{
			delete FireCharge;
			return;
		}
		m_World->BroadcastSpawnEntity(*FireCharge);
		m_AttackInterval = 0.0;
		// ToDo: Shoot 3 fireballs instead of 1.
	}
}