#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorItemReplacer.h"
#include "../Monster.h"
#include "../Entities/Player.h"


cBehaviorItemReplacer::cBehaviorItemReplacer(short a_OriginalItem, short a_NewItem) :
	m_OriginalItem(a_OriginalItem) ,
	m_NewItem(a_NewItem)
{

}





void cBehaviorItemReplacer::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachRightClickBehavior(this);
}





void cBehaviorItemReplacer::OnRightClicked(cPlayer & a_Player)
{
	short HeldItem = a_Player.GetEquippedItem().m_ItemType;
	if (HeldItem == m_OriginalItem)
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
			a_Player.GetInventory().AddItem(m_NewItem);
		}
	}
}
