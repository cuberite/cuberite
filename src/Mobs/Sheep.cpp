
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
	// The sheep should not move when he's eating so only handle the physics.
	if (m_TimeToStopEating > 0)
	{
		HandlePhysics(a_Dt, a_Chunk);
		m_TimeToStopEating--;
		if (m_TimeToStopEating == 0)
		{
			if (m_World->GetBlock((int) GetPosX(), (int) GetPosY() - 1, (int) GetPosZ()) == E_BLOCK_GRASS)
			{
				// The sheep ate the grass so we change it to dirt.
				m_World->SetBlock((int) GetPosX(), (int) GetPosY() - 1, (int) GetPosZ(), E_BLOCK_DIRT, 0);
				m_IsSheared = false;
				m_World->BroadcastEntityMetadata(*this);
			}
		}
	}
	else
	{
		super::Tick(a_Dt, a_Chunk);
		if (m_World->GetTickRandomNumber(600) == 1)
		{
			if (m_World->GetBlock((int) GetPosX(), (int) GetPosY() - 1, (int) GetPosZ()) == E_BLOCK_GRASS)
			{
				m_World->BroadcastEntityStatus(*this, 10);
				m_TimeToStopEating = 40;
			}
		}
	}
	cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), (float)m_SightDistance);
	if (a_Closest_Player != NULL)
	{
		if (a_Closest_Player->GetEquippedItem().m_ItemType == E_ITEM_WHEAT)
		{
			if (!IsBegging())
			{
				m_IsBegging = true;
				m_World->BroadcastEntityMetadata(*this);
			}
			Vector3d PlayerPos = a_Closest_Player->GetPosition();
			PlayerPos.y++;
			m_FinalDestination = PlayerPos;
		}
		else
		{
			if (IsBegging())
			{
				m_IsBegging = false;
				m_World->BroadcastEntityMetadata(*this);
			}
		}
	}
}

