
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChestEntity.h"
#include "Item.h"
#include "ClientHandle.h"
#include "Player.h"
#include "UI/Window.h"
#include "World.h"
#include "Root.h"
#include "Pickup.h"
#include "Noise.h"
#include <json/json.h>





class cWorld;
class cRoot;





cChestEntity::cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ) :
	super(E_BLOCK_CHEST, a_BlockX, a_BlockY, a_BlockZ),
	m_Contents(c_ChestWidth, c_ChestHeight)
{
	cBlockEntityWindowOwner::SetBlockEntity(this);
}





cChestEntity::cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_CHEST, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Contents(c_ChestWidth, c_ChestHeight)
{
	cBlockEntityWindowOwner::SetBlockEntity(this);
}





cChestEntity::~cChestEntity()
{
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->OwnerDestroyed();
	}
}





void cChestEntity::Destroy(void)
{
	// Drop items
	cItems Pickups;
	m_Contents.CopyToItems(Pickups);
	m_Contents.Clear();
	m_World->SpawnItemPickups(Pickups, m_PosX, m_PosY, m_PosZ);
}





bool cChestEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for (Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr)
	{
		cItem Item;
		Item.FromJson( *itr );
		SetSlot( SlotIdx, Item );
		SlotIdx++;
	}
	return true;
}





void cChestEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	Json::Value AllSlots;
	for (int i = m_Contents.GetNumSlots(); i >= 0; i--)
	{
		Json::Value Slot;
		m_Contents.GetItem(i).GetJson(Slot);
		AllSlots.append(Slot);
	}
	a_Value["Slots"] = AllSlots;
}





void cChestEntity::SendTo(cClientHandle & a_Client)
{
	// The chest entity doesn't need anything sent to the client when it's created / gets in the viewdistance
	// All the actual handling is in the cWindow UI code that gets called when the chest is rclked
	
	UNUSED(a_Client);
}





void cChestEntity::UsedBy(cPlayer * a_Player)
{
	if (GetWindow() == NULL)
	{
		OpenNewWindow();
	}
	if (GetWindow())
	{
		if( a_Player->GetWindow() != GetWindow() )
		{
			a_Player->OpenWindow( GetWindow() );
			GetWindow()->SendWholeWindow(*a_Player->GetClientHandle());
		}
	}

	// This is rather a hack
	// Instead of marking the chunk as dirty upon chest contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the chest first.
	// The few false positives aren't much to worry about
	int ChunkX, ChunkY = 0, ChunkZ;
	cChunkDef::BlockToChunk(m_PosX, m_PosY, m_PosZ, ChunkX, ChunkZ);
	m_World->MarkChunkDirty(ChunkX, ChunkY, ChunkZ);
}





void cChestEntity::OpenNewWindow(void)
{
	// Callback for opening together with neighbor chest:
	class cOpenDouble :
		public cChestCallback
	{
		cChestEntity * m_ThisChest;
	public:
		cOpenDouble(cChestEntity * a_ThisChest) :
			m_ThisChest(a_ThisChest)
		{
		}
		
		virtual bool Item(cChestEntity * a_Chest) override
		{
			// The primary chest should eb the one with lesser X or Z coord:
			cChestEntity * Primary = a_Chest;
			cChestEntity * Secondary = m_ThisChest;
			if (
				(Primary->GetPosX() > Secondary->GetPosX()) ||
				(Primary->GetPosZ() > Secondary->GetPosZ())
			)
			{
				std::swap(Primary, Secondary);
			}
			m_ThisChest->OpenWindow(new cChestWindow(Primary, Secondary));
			return false;
		}
	} ;
	
	// Scan neighbors for adjacent chests:
	cOpenDouble OpenDbl(this);
	if (
		m_World->DoWithChestAt(m_PosX - 1, m_PosY, m_PosZ,     OpenDbl) ||
		m_World->DoWithChestAt(m_PosX + 1, m_PosY, m_PosZ,     OpenDbl) ||
		m_World->DoWithChestAt(m_PosX    , m_PosY, m_PosZ - 1, OpenDbl) ||
		m_World->DoWithChestAt(m_PosX    , m_PosY, m_PosZ + 1, OpenDbl)
	)
	{
		// The double-chest window has been opened in the callback
		return;
	}

	// There is no chest neighbor, open a single-chest window:	
	OpenWindow(new cChestWindow(this));
}




