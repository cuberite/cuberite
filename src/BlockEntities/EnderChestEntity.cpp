
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderChestEntity.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/Window.h"
#include "json/json.h"





cEnderChestEntity::cEnderChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_ENDER_CHEST, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World)
{
	cBlockEntityWindowOwner::SetBlockEntity(this);
}





cEnderChestEntity::~cEnderChestEntity()
{
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->OwnerDestroyed();
	}
}





bool cEnderChestEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for (Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr)
	{
		cItem Item;
		Item.FromJson(*itr);
		SetSlot(SlotIdx, Item);
		SlotIdx++;
	}
	return true;
}





void cEnderChestEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	Json::Value AllSlots;
	for (int i = m_Contents.GetNumSlots() - 1; i >= 0; i--)
	{
		Json::Value Slot;
		m_Contents.GetSlot(i).GetJson(Slot);
		AllSlots.append(Slot);
	}
	a_Value["Slots"] = AllSlots;
}





void cEnderChestEntity::SendTo(cClientHandle & a_Client)
{
	// The chest entity doesn't need anything sent to the client when it's created / gets in the viewdistance
	// All the actual handling is in the cWindow UI code that gets called when the chest is rclked
	
	UNUSED(a_Client);
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

	// This is rather a hack
	// Instead of marking the chunk as dirty upon chest contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the chest first.
	// The few false positives aren't much to worry about
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(m_PosX, m_PosZ, ChunkX, ChunkZ);
	m_World->MarkChunkDirty(ChunkX, ChunkZ);
}





void cEnderChestEntity::OpenNewWindow(void)
{
	OpenWindow(new cEnderChestWindow(this));
}




