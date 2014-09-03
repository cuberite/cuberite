
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderChestEntity.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/Window.h"
#include "json/json.h"





cEnderChestEntity::cEnderChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_ENDER_CHEST, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
}





cEnderChestEntity::~cEnderChestEntity()
{
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->OwnerDestroyed();
	}
}





void cEnderChestEntity::UsedBy(cPlayer * a_Player)
{
	// If the window is not created, open it anew:
	cWindow * Window = GetWindow();
	if (Window == NULL)
	{
		OpenNewWindow();
		Window = GetWindow();
	}
	
	// Open the window for the player:
	if (Window != NULL)
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




