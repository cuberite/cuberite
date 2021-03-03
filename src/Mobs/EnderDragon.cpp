
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderDragon.h"
#include "../ClientHandle.h"
#include "../CompositeChat.h"





cEnderDragon::cEnderDragon(void) :
	// TODO: Vanilla source says this, but is it right? Dragons fly, they don't stand
	Super("EnderDragon", mtEnderDragon, "entity.enderdragon.hurt", "entity.enderdragon.death", "entity.enderdragon.ambient", 16.0, 8.0)
{
}





bool cEnderDragon::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	m_World->BroadcastBossBarUpdateHealth(GetUniqueID(), GetHealth() / GetMaxHealth());
	return true;
}





void cEnderDragon::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// No drops
}





void cEnderDragon::SpawnOn(cClientHandle & a_Client)
{
	Super::SpawnOn(a_Client);

	// Red boss bar with no divisions that plays boss music and creates fog:
	a_Client.SendBossBarAdd(GetUniqueID(), cCompositeChat("Ender Dragon"), GetHealth() / GetMaxHealth(), BossBarColor::Red, BossBarDivisionType::None, false, true, true);
}
