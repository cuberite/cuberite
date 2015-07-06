
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderChestEntity.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/EnderChestWindow.h"





cEnderChestEntity::cEnderChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_ENDER_CHEST, a_BlockX, a_BlockY, a_BlockZ, a_World),
	cBlockEntityWindowOwner(this)
{
}





cEnderChestEntity::~cEnderChestEntity()
{
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cEnderChestEntity::UsedBy(cPlayer * a_Player)
{
	// TODO: cats are an obstruction
	if ((GetPosY() < cChunkDef::Height - 1) && !cBlockInfo::IsTransparent(GetWorld()->GetBlock(GetPosX(), GetPosY() + 1, GetPosZ())))
	{
		// Obstruction, don't open
		return;
	}
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
			a_Player->OpenWindow(Window);
		}
	}
}





void cEnderChestEntity::OpenNewWindow()
{
	OpenWindow(new cEnderChestWindow(this));
}





void cEnderChestEntity::LoadFromJson(const Json::Value & a_Value, cItemGrid & a_Grid)
{
	int SlotIdx = 0;
	for (Json::Value::iterator itr = a_Value.begin(); itr != a_Value.end(); ++itr)
	{
		cItem Item;
		Item.FromJson(*itr);
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
