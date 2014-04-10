
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Sheep.h"
#include "../BlockID.h"
#include "../Entities/Player.h"
#include "../World.h"





cSheep::cSheep(int a_Color) :
	super("Sheep", mtSheep, "mob.sheep.say", "mob.sheep.say", 0.6, 1.3),
	m_IsSheared(false),
	m_WoolColor(a_Color),
	m_TimeToStopEating(-1)
{
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
	if ((a_Player.GetEquippedItem().m_ItemType == E_ITEM_SHEARS) && (!m_IsSheared))
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
	}
	if ((a_Player.GetEquippedItem().m_ItemType == E_ITEM_DYE) && (m_WoolColor != 15 - a_Player.GetEquippedItem().m_ItemDamage))
	{
		m_WoolColor = 15 - a_Player.GetEquippedItem().m_ItemDamage;
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
				m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_SHEEP_EATING);
				m_TimeToStopEating = 40;
			}
		}
	}
}

