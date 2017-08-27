
#pragma once

#include "Window.h"
#include "SlotArea.h"
#include "../Mobs/Villager.h"





class cVillagerTradeWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cVillagerTradeWindow(cVillager & Villager) :
		cWindow(wtVillagerTrade, Printf("Villager № %i", Villager.GetUniqueID())),
		m_Villager(Villager)
	{
		m_SlotAreas.push_back(new cSlotAreaVillagerTrade(m_Villager, *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override
	{
		cSlotAreas AreasInOrder;

		if (a_ClickedArea == m_SlotAreas[0])
		{
			// Trading Area
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
		else if (a_ClickedArea == m_SlotAreas[1])
		{
			// Inventory
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
		else
		{
			// Hotbar
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
	}

	virtual void OpenedByPlayer(cPlayer & Player) override
	{
		super::OpenedByPlayer(Player);
		m_TradeIndexOpenedByPlayer[&Player] = 0;
		m_Villager.HandleTradeInProgress();
	}

	void PlayerChangedTradeOffer(const cPlayer & Player, unsigned NewIndex)
	{
		m_TradeIndexOpenedByPlayer[&Player] = NewIndex;
		static_cast<cSlotAreaVillagerTrade *>(m_SlotAreas[0])->UpdateTrade(Player);
		m_Villager.HandleTradeInProgress();
	}

	unsigned GetPlayerTradeOfferIndex(const cPlayer & Player)
	{
		return m_TradeIndexOpenedByPlayer[&Player];
	}

private:
	cVillager & m_Villager;
	std::unordered_map<const cPlayer *, unsigned> m_TradeIndexOpenedByPlayer;
};
