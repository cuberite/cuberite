
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Slime.h"
#include "FastRandom.h"
#include "World.h"





cSlime::cSlime(int a_Size) :
	super("Slime",
		mtSlime,
		Printf("entity.%sslime.hurt", GetSizeName(a_Size).c_str()),
		Printf("entity.%sslime.death", GetSizeName(a_Size).c_str()),
		0.6 * a_Size,
		0.6 * a_Size
	),
	m_Size(a_Size)
{
	SetMaxHealth(a_Size * a_Size);
	SetAttackDamage(a_Size);
}





void cSlime::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}

	// Only slimes with the size 1 can drop slimeballs.
	if (m_Size == 1)
	{
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_SLIMEBALL);
	}
}





bool cSlime::Attack(std::chrono::milliseconds a_Dt)
{
	if (m_Size > 1)
	{
		// Only slimes larger than size 1 attack a player.
		return super::Attack(a_Dt);
	}

	return false;
}





void cSlime::KilledBy(TakeDamageInfo & a_TDI)
{
	if (GetHealth() > 0)
	{
		return;
	}

	if (m_Size != 1)
	{
		auto & Random = GetRandomProvider();
		int SpawnAmount = Random.RandInt(2, 4);

		for (int i = 0; i < SpawnAmount; ++i)
		{
			double AddX = (i % 2 - 0.5) * m_Size / 4.0;
			double AddZ = (i / 2 - 0.5) * m_Size / 4.0;

			auto NewSlime = cpp14::make_unique<cSlime>(m_Size / 2);
			NewSlime->SetPosition(GetPosX() + AddX, GetPosY() + 0.5, GetPosZ() + AddZ);
			NewSlime->SetYaw(Random.RandReal(360.0f));
			m_World->SpawnMobFinalize(std::move(NewSlime));
		}
	}
	super::KilledBy(a_TDI);
}





AString cSlime::GetSizeName(int a_Size)
{
	if (a_Size == 1)
	{
		return "small_";
	}
	return "";
}




