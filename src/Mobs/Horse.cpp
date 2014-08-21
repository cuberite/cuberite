#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Horse.h"
#include "../World.h"
#include "../Entities/Player.h"





cHorse::cHorse(int Type, int Color, int Style, int TameTimes) :
	super("Horse", mtHorse, "mob.horse.hit", "mob.horse.death", 1.4, 1.6),
	m_bHasChest(false),
	m_bIsEating(false),
	m_bIsRearing(false),
	m_bIsMouthOpen(false),
	m_bIsTame(false),
	m_bIsSaddled(false),
	m_Type(Type),
	m_Color(Color),
	m_Style(Style),
	m_Armour(0),
	m_TimesToTame(TameTimes),
	m_TameAttemptTimes(0),
	m_RearTickCount(0)
{
}





void cHorse::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (!m_bIsMouthOpen)
	{
		if (m_World->GetTickRandomNumber(50) == 25)
		{
			m_bIsMouthOpen = true;
		}
	}
	else
	{
		if (m_World->GetTickRandomNumber(10) == 5)
		{
			m_bIsMouthOpen = false;
		}
	}

	if ((m_Attachee != NULL) && (!m_bIsTame))
	{
		if (m_TameAttemptTimes < m_TimesToTame)
		{
			if (m_World->GetTickRandomNumber(50) == 25)
			{
				m_World->BroadcastSoundParticleEffect(2000, (int)GetPosX(), (int)GetPosY(), (int)GetPosZ(), 0);
				m_World->BroadcastSoundParticleEffect(2000, (int)GetPosX(), (int)GetPosY(), (int)GetPosZ(), 2);
				m_World->BroadcastSoundParticleEffect(2000, (int)GetPosX(), (int)GetPosY(), (int)GetPosZ(), 6);
				m_World->BroadcastSoundParticleEffect(2000, (int)GetPosX(), (int)GetPosY(), (int)GetPosZ(), 8);

				m_Attachee->Detach();
				m_bIsRearing = true;
			}
		}
		else
		{
			m_bIsTame = true;
		}
	}
	
	if (m_bIsRearing)
	{
		if (m_RearTickCount == 20)
		{
			m_bIsRearing = false;
			m_RearTickCount = 0;
		}
		else
		{
			m_RearTickCount++;
		}
	}

	m_World->BroadcastEntityMetadata(*this);
}





void cHorse::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	if (m_bIsSaddled)
	{
		a_Drops.push_back(cItem(E_ITEM_SADDLE, 1));
	}
}




