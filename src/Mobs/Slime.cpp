
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Slime.h"
#include "../FastRandom.h"
#include "../World.h"





cSlime::cSlime(int a_Size) :
	Super("Slime",
		mtSlime,
		fmt::format(FMT_STRING("entity.{}slime.hurt"),  GetSizeName(a_Size)),
		fmt::format(FMT_STRING("entity.{}slime.death"), GetSizeName(a_Size)),
		"",
		0.51f * a_Size,
		0.51f * a_Size
	),
	m_Size(a_Size)
{
	SetMaxHealth(static_cast<float>(a_Size * a_Size));
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
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, Item::SlimeBall);
	}
}





bool cSlime::Attack(std::chrono::milliseconds a_Dt)
{
	if (m_Size > 1)
	{
		// Only slimes larger than size 1 attack a player.
		return Super::Attack(a_Dt);
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

			// Queue slimes to be spawned after the 1 second death animation has finished playing:
			m_World->ScheduleTask(cTickTime(20), [
				Position = GetPosition() + Vector3d(AddX, 0.5, AddZ),
				Yaw = Random.RandReal(360.0f),
				Size = m_Size / 2
			](cWorld & a_World)
			{
				auto NewSlime = std::make_unique<cSlime>(Size);
				NewSlime->SetPosition(Position);
				NewSlime->SetYaw(Yaw);
				a_World.SpawnMobFinalize(std::move(NewSlime));
			});
		}
	}
	Super::KilledBy(a_TDI);
}





AString cSlime::GetSizeName(int a_Size)
{
	if (a_Size == 1)
	{
		return "small_";
	}
	return "";
}




