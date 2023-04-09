
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderChestEntity.h"
#include "json/json.h"
#include "../BlockInfo.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/EnderChestWindow.h"
#include "../ClientHandle.h"
#include "../Mobs/Ocelot.h"





cEnderChestEntity::cEnderChestEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World),
	cBlockEntityWindowOwner(this)
{
	ASSERT(a_Block.Type() == BlockType::EnderChest);
}





void cEnderChestEntity::SendTo(cClientHandle & a_Client)
{
	// Send a dummy "number of players with chest open" packet to make the chest visible:
	a_Client.SendBlockAction(m_Pos, 1, 0, m_Block.Type());
}





void cEnderChestEntity::OnRemoveFromWorld()
{
	const auto Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





bool cEnderChestEntity::UsedBy(cPlayer * a_Player)
{
	if (
		(GetPosY() < cChunkDef::Height - 1) &&
		(
			!cBlockInfo::IsTransparent(GetWorld()->GetBlock(GetPos().addedY(1))) ||
			!cOcelot::IsCatSittingOnBlock(GetWorld(), Vector3d(GetPos()))
		)
	)
	{
		// Obstruction, don't open
		return false;
	}

	a_Player->GetStatistics().Custom[CustomStatistic::OpenEnderchest]++;

	// If the window is not created, open it anew:
	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenNewWindow();
		Window = GetWindow();
	}

	// Open the window for the player:
	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(*Window);
		}
	}
	return true;
}





void cEnderChestEntity::OpenNewWindow()
{
	OpenWindow(new cEnderChestWindow(this));
}





void cEnderChestEntity::LoadFromJson(const Json::Value & a_Value, cItemGrid & a_Grid)
{
	int SlotIdx = 0;
	for (auto & Node : a_Value)
	{
		cItem Item;
		Item.FromJson(Node);
		a_Grid.SetSlot(SlotIdx, Item);
		SlotIdx++;
	}
}





void cEnderChestEntity::SaveToJson(Json::Value & a_Value, const cItemGrid & a_Grid)
{
	for (int i = 0; i < a_Grid.GetNumSlots(); i++)
	{
		Json::Value Slot;
		a_Grid.GetSlot(i).GetJson(Slot);
		a_Value.append(Slot);
	}
}
