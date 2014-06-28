
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Sheep.h"
#include "../BlockID.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "FastRandom.h"





cSheep::cSheep(int a_Color) :
	super("Sheep", mtSheep, "mob.sheep.say", "mob.sheep.say", 0.6, 1.3),
	m_IsSheared(false),
	m_WoolColor(a_Color),
	m_TimeToStopEating(-1)
{
	// Generate random wool color.
	if (m_WoolColor == -1)
	{
		cFastRandom Random;
		int Chance = Random.NextInt(101);

		if (Chance <= 81)
		{
			// White
			m_WoolColor = 0;
		}
		else if (Chance <= 86)
		{
			// Black
			m_WoolColor = 15;
		}
		else if (Chance <= 91)
		{
			// Grey
			m_WoolColor = 7;
		}
		else if (Chance <= 96)
		{
			// Light grey
			m_WoolColor = 8;
		}
		else if (Chance <= 99)
		{
			// Brown
			m_WoolColor = 12;
		}
		else
		{
			// Pink
			m_WoolColor = 6;
		}
	}
}





void cSheep::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (!m_IsSheared)
	{
		a_Drops.push_back(cItem(E_BLOCK_WOOL, 1, m_WoolColor));
	}
}





void cSheep::OnRightClicked(cPlayer & a_Player)
{
	const cItem & EquippedItem = a_Player.GetEquippedItem();
	if ((EquippedItem.m_ItemType == E_ITEM_SHEARS) && !IsSheared() && !IsBaby())
	{
		m_IsSheared = true;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.UseEquippedItem();
		}

		cItems Drops;
		int NumDrops = m_World->GetTickRandomNumber(2) + 1;
		Drops.push_back(cItem(E_BLOCK_WOOL, NumDrops, m_WoolColor));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
		m_World->BroadcastSoundEffect("mob.sheep.shear", POSX_TOINT * 8, POSY_TOINT * 8, POSZ_TOINT * 8, 1.0f, 1.0f);
	}
	else if ((EquippedItem.m_ItemType == E_ITEM_DYE) && (m_WoolColor != 15 - EquippedItem.m_ItemDamage))
	{
		m_WoolColor = 15 - EquippedItem.m_ItemDamage;
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cSheep::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	int PosX = POSX_TOINT;
	int PosY = POSY_TOINT - 1;
	int PosZ = POSZ_TOINT;

	if ((PosY <= 0) || (PosY > cChunkDef::Height))
	{
		return;
	}

	if (m_TimeToStopEating > 0)
	{
		m_bMovingToDestination = false; // The sheep should not move when he's eating
		m_TimeToStopEating--;

		if (m_TimeToStopEating == 0)
		{
			if (m_World->GetBlock(PosX, PosY, PosZ) == E_BLOCK_GRASS) // Make sure grass hasn't been destroyed in the meantime
			{
				// The sheep ate the grass so we change it to dirt
				m_World->SetBlock(PosX, PosY, PosZ, E_BLOCK_DIRT, 0);
				GetWorld()->BroadcastSoundParticleEffect(2001, PosX, PosY, PosX, E_BLOCK_GRASS);
				m_IsSheared = false;
				m_World->BroadcastEntityMetadata(*this);
			}
		}
	}
	else
	{
		if (m_World->GetTickRandomNumber(600) == 1)
		{
			if (m_World->GetBlock(PosX, PosY, PosZ) == E_BLOCK_GRASS)
			{
				m_World->BroadcastEntityStatus(*this, esSheepEating);
				m_TimeToStopEating = 40;
			}
		}
	}
}

