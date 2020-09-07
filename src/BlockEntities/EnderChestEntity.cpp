
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderChestEntity.h"
#include "json/json.h"
#include "../BlockInfo.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/EnderChestWindow.h"
#include "../ClientHandle.h"
#include "../Mobs/Ocelot.h"





cEnderChestEntity::cEnderChestEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World),
	cBlockEntityWindowOwner(this)
{
	ASSERT(a_BlockType == E_BLOCK_ENDER_CHEST);
}





cEnderChestEntity::~cEnderChestEntity()
{
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cEnderChestEntity::SendTo(cClientHandle & a_Client)
{
	// Send a dummy "number of players with chest open" packet to make the chest visible:
	a_Client.SendBlockAction(m_Pos.x, m_Pos.y, m_Pos.z, 1, 0, m_BlockType);
}





bool cEnderChestEntity::UsedBy(cPlayer * a_Player)
{
	if (
		(GetPosY() < cChunkDef::Height - 1) &&
		(
			!cBlockInfo::IsTransparent(GetWorld()->GetBlock(GetPosX(), GetPosY() + 1, GetPosZ())) ||
			!cOcelot::IsCatSittingOnBlock(GetWorld(), Vector3d(GetPos()))
		)
	)
	{
		// Obstruction, don't open
		return false;
	}

	a_Player->GetStatManager().AddValue(Statistic::OpenEnderchest);

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
