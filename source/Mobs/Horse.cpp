
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Horse.h"
#include "../World.h"
#include "../Entities/Player.h"





cHorse::cHorse(int Type, int Color, int Style, int TameTimes) :
	super("Horse", 100, "mob.horse.hit", "mob.horse.death", 1.4, 1.6),
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
				m_World->BroadcastSoundParticleEffect(2000, (int)(floor(GetPosX()) * 8), (int)(floor(GetPosY()) * 8), (int)(floor(GetPosZ()) * 8), 0);
				m_World->BroadcastSoundParticleEffect(2000, (int)(floor(GetPosX()) * 8), (int)(floor(GetPosY()) * 8), (int)(floor(GetPosZ()) * 8), 2);
				m_World->BroadcastSoundParticleEffect(2000, (int)(floor(GetPosX()) * 8), (int)(floor(GetPosY()) * 8), (int)(floor(GetPosZ()) * 8), 6);
				m_World->BroadcastSoundParticleEffect(2000, (int)(floor(GetPosX()) * 8), (int)(floor(GetPosY()) * 8), (int)(floor(GetPosZ()) * 8), 8);

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
		}
		else { m_RearTickCount++;}
	}

	m_World->BroadcastEntityMetadata(*this);
}





void cHorse::OnRightClicked(cPlayer & a_Player)
{
	if (m_Attachee != NULL)
	{
		if (m_Attachee->GetUniqueID() == a_Player.GetUniqueID())
		{
			a_Player.Detach();
			return;
		}
		
		if (m_Attachee->IsPlayer())
		{
			return;
		}
		
		m_Attachee->Detach();
	}
	
	if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_SADDLE)
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}

		// Set saddle state & broadcast metadata
		m_bIsSaddled = true;
		m_World->BroadcastEntityMetadata(*this);
	}
	else
	{
		m_TameAttemptTimes++;
	        a_Player.AttachTo(this);
	}
}





void cHorse::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_LEATHER);
}




