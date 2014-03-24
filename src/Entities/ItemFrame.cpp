/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ItemFrame.h"
#include "ClientHandle.h"
#include "Player.h"





cItemFrame::cItemFrame(eBlockFace a_BlockFace, double a_X, double a_Y, double a_Z)
	:	cHangingEntity(etItemFrame, a_BlockFace, a_X, a_Y, a_Z)
	, m_Item(E_BLOCK_AIR)
	, m_Rotation(0)
{
}





void cItemFrame::OnRightClicked(cPlayer & a_Player)
{
	if (!m_Item.IsEmpty())
	{
		// Item not empty, rotate, clipping values to zero to three inclusive
		m_Rotation++;
		if (m_Rotation >= 4)
		{
			m_Rotation = 0;
		}
	}
	else if (!a_Player.GetEquippedItem().IsEmpty())
	{
		// Item empty, and player held item not empty - add this item to self
		m_Item = a_Player.GetEquippedItem();
		m_Item.m_ItemCount = 1;

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}

	GetWorld()->BroadcastEntityMetadata(*this); // Update clients
}






void cItemFrame::KilledBy(cEntity * a_Killer)
{
	if (m_Item.IsEmpty())
	{
		super::KilledBy(a_Killer);
		Destroy();
		return;
	}

	if ((a_Killer != NULL) && a_Killer->IsPlayer() && !((cPlayer *)a_Killer)->IsGameModeCreative())
	{
		cItems Item;
		Item.push_back(m_Item);

		GetWorld()->SpawnItemPickups(Item, GetPosX(), GetPosY(), GetPosZ());
	}

	SetHealth(GetMaxHealth());
	m_Item.Clear();
	m_Rotation = 0;
	GetWorld()->BroadcastEntityMetadata(*this);
}





void cItemFrame::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != NULL) && a_Killer->IsPlayer() && !((cPlayer *)a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_ITEM_FRAME));
	}
}




