
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cChestEntity.h"
#include "cItem.h"
#include "cClientHandle.h"
#include "cPlayer.h"
#include "cWindow.h"
#include "cWorld.h"
#include "cRoot.h"
#include "cPickup.h"
#include <json/json.h>





class cWorld;
class cRoot;





cChestEntity::cChestEntity(int a_X, int a_Y, int a_Z, cWorld * a_World)
	: cBlockEntity( E_BLOCK_CHEST, a_X, a_Y, a_Z, a_World)
	, m_TopChest( false )
	, m_JoinedChest( NULL )
{
	m_Content = new cItem[ c_ChestHeight * c_ChestWidth ];
}





cChestEntity::~cChestEntity()
{
	if( GetWindow() )
	{
		GetWindow()->OwnerDestroyed();
	}

	if( m_Content )
	{
		delete [] m_Content;
	}
}





void cChestEntity::Destroy()
{
	// Drop items
	for( int i = 0; i < c_ChestHeight * c_ChestWidth; ++i )
	{
		if( !m_Content[i].IsEmpty() )
		{
			cPickup * Pickup = new cPickup( m_PosX * 32 + 16, m_PosY * 32 + 16, m_PosZ * 32 + 16, m_Content[i], 0, 1.f, 0 );
			Pickup->Initialize(m_World);
			m_Content[i].Empty();
		}
	}
	if (m_JoinedChest)
	{
		m_JoinedChest->RemoveJoinedChest(this);
	}
}





const cItem * cChestEntity::GetSlot( int a_Slot ) const
{
	if( a_Slot > -1 && a_Slot < c_ChestHeight*c_ChestWidth )
	{
		return &m_Content[ a_Slot ];
	}
	return 0;
}





void cChestEntity::SetSlot( int a_Slot, cItem & a_Item )
{
	if( a_Slot > -1 && a_Slot < c_ChestHeight*c_ChestWidth )
	{
		m_Content[a_Slot] = a_Item;
	}
}





#define READ(File, Var) \
	if (File.Read(&Var, sizeof(Var)) != sizeof(Var)) \
	{ \
		LOGERROR("ERROR READING cChestEntity %s FROM FILE (line %d)", #Var, __LINE__); \
		return false; \
	}

bool cChestEntity::LoadFromFile(cFile & f)
{
	READ(f, m_PosX);
	READ(f, m_PosY);
	READ(f, m_PosZ);

	unsigned int NumSlots = 0;
	READ(f, NumSlots);
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		cItem Item;
		READ(f, Item.m_ItemID);
		READ(f, Item.m_ItemCount);
		READ(f, Item.m_ItemHealth);
		SetSlot( i, Item );
	}
	return true;
}





bool cChestEntity::LoadFromJson( const Json::Value& a_Value )
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for( Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr )
	{
		cItem Item;
		Item.FromJson( *itr );
		SetSlot( SlotIdx, Item );
		SlotIdx++;
	}
	return true;
}





void cChestEntity::SaveToJson( Json::Value& a_Value )
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	unsigned int NumSlots = c_ChestHeight*c_ChestWidth;
	Json::Value AllSlots;
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		Json::Value Slot;
		const cItem * Item = GetSlot( i );
		if( Item ) Item->GetJson( Slot );
		AllSlots.append( Slot );
	}
	a_Value["Slots"] = AllSlots;
}





void cChestEntity::SendTo( cClientHandle* a_Client, cServer* a_Server )
{
	(void)a_Client;
	(void)a_Server;
	return;
}





void cChestEntity::UsedBy( cPlayer * a_Player )
{
	if( !GetWindow() )
	{
		cWindow* Window = new cWindow( this, true );
		Window->SetSlots( GetContents(), GetChestHeight()*c_ChestWidth );
		Window->SetWindowID( 1 );
		Window->SetWindowType( cWindow::Chest );
		Window->SetWindowTitle("UberChest");
		Window->GetOwner()->SetEntity(this);
		OpenWindow( Window );
	}
	if ( GetWindow() )
	{
		if( a_Player->GetWindow() != GetWindow() )
		{
			a_Player->OpenWindow( GetWindow() );
			GetWindow()->SendWholeWindow( a_Player->GetClientHandle() );
		}
	}
	cPacket_BlockAction ChestOpen;
	ChestOpen.m_PosX = GetPosX();
	ChestOpen.m_PosY = (short)GetPosY();
	ChestOpen.m_PosZ = GetPosZ();
	ChestOpen.m_Byte1 = (char)1;
	ChestOpen.m_Byte2 = (char)1;
	m_World->BroadcastToChunkOfBlock(m_PosX, m_PosY, m_PosZ, &ChestOpen);

	// This is rather a hack
	// Instead of marking the chunk as dirty upon chest contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the chest first.
	// The few false positives aren't much to worry about
	int ChunkX, ChunkY = 0, ChunkZ;
	cChunkDef::BlockToChunk(m_PosX, m_PosY, m_PosZ, ChunkX, ChunkZ);
	m_World->MarkChunkDirty(ChunkX, ChunkY, ChunkZ);
}





cItem *cChestEntity::GetContents(bool a_OnlyThis)
{
	if (m_JoinedChest && !a_OnlyThis)
	{
		cItem *Combined = new cItem[GetChestHeight()*c_ChestWidth];
		int i;
		cItem *first = (m_TopChest) ? GetContents(true) : m_JoinedChest->GetContents(true);
		cItem *second = (!m_TopChest) ? GetContents(true) :  m_JoinedChest->GetContents(true);
		for (i=0; i < GetChestHeight()*c_ChestWidth; i++)
		{
			int index = i % c_ChestHeight*c_ChestWidth;
			if (i < c_ChestHeight*c_ChestWidth)
				Combined[index] = first[index];
			else
				Combined[index] = second[index];
		}
		return Combined;
	}
	else
		return m_Content;
}




