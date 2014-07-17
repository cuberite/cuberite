
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Slime.h"
#include "FastRandom.h"
#include "World.h"





/// Creates a slime of the specified size; size is 1 .. 3, with 1 being the smallest
cSlime::cSlime(int a_Size) :
	super("Slime", mtSlime, Printf("mob.slime.%s", GetSizeName(a_Size).c_str()), Printf("mob.slime.%s", GetSizeName(a_Size).c_str()), 0.6 * a_Size, 0.6 * a_Size),
	m_Size(a_Size)
{
	SetMaxHealth(a_Size * a_Size);
	SetAttackDamage(a_Size);
}





void cSlime::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}

	if (GetSize() == 1)
	{
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_SLIMEBALL);
	}
}





void cSlime::Attack(float a_Dt)
{
	if (m_Size != 1)
	{
		// Only slimes with the size 2 and 3 attacks a player.
		super::Attack(a_Dt);
	}
}





void cSlime::KilledBy(TakeDamageInfo & a_TDI)
{
	if (GetHealth() > 0)
	{
		return;
	}

	if (m_Size != 1)
	{
		cFastRandom Random;
		int SpawnAmount = 2 + Random.NextInt(3);

		for (int i = 0; i < SpawnAmount; ++i)
		{
			double AddX = (i % 2 - 0.5) * m_Size / 4.0;
			double AddZ = (i / 2 - 0.5) * m_Size / 4.0;

			cSlime * NewSlime = new cSlime(m_Size / 2);
			NewSlime->SetPosition(GetPosX() + AddX, GetPosY() + 0.5, GetPosZ() + AddZ);
			NewSlime->SetYaw(Random.NextFloat(2.0f) * 360.0f);
			NewSlime->SetPitch(0.0f);

			m_World->SpawnMobFinalize(NewSlime);
		}
	}
	super::KilledBy(a_TDI);
}





const AString & cSlime::GetSizeName(int a_Size)
{
	if (a_Size > 1)
	{
		return "big";
	}
	else
	{
		return "small";
	}
}




