
// FlowerPotEntity.cpp

// Implements the cFlowerPotEntity class representing a single flower pot in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "FlowerPotEntity.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"
#include "../Item.h"
#include "../Blocks/BlockFlowerPot.h"





cFlowerPotEntity::cFlowerPotEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World)
{
	ASSERT(cBlockFlowerPotHandler::IsBlockFlowerPot(a_Block));
}





cItems cFlowerPotEntity::ConvertToPickups() const
{
	return cItem(m_Item);
}





void cFlowerPotEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cFlowerPotEntity &>(a_Src);
	m_Item = src.m_Item;
}





bool cFlowerPotEntity::UsedBy(cPlayer * a_Player)
{
	if (IsItemInPot())
	{
		return false;
	}

	a_Player->GetStatistics().Custom[CustomStatistic::PotFlower]++;

	cItem SelectedItem = a_Player->GetInventory().GetEquippedItem();
	if (IsFlower(SelectedItem.m_ItemType))
	{
		m_Item = SelectedItem.CopyOne();
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}
		m_World->BroadcastBlockEntity(GetPos(), a_Player->GetClientHandle());
	}
	return true;
}





void cFlowerPotEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





bool cFlowerPotEntity::IsFlower(Item m_ItemType)
{
	switch (m_ItemType)
	{
		case Item::Dandelion:
		case Item::Cactus:
		case Item::BrownMushroom:
		case Item::RedMushroom:
		case Item::AcaciaSapling:
		case Item::BirchSapling:
		case Item::JungleSapling:
		case Item::DarkOakSapling:
		case Item::OakSapling:
		case Item::SpruceSapling:
		case Item::DeadBush:
		case Item::Lilac:
		case Item::Peony:
		case Item::RoseBush:
		case Item::Sunflower:
		case Item::CherrySapling:
		case Item::MangrovePropagule:
		case Item::Azalea:
		case Item::FloweringAzalea:
		case Item::Torchflower:
			return true;
		default: return false;
	}
}
