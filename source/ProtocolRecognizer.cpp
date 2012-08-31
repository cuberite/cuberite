
// ProtocolRecognizer.cpp

// Implements the cProtocolRecognizer class representing the meta-protocol that recognizes possibly multiple 
// protocol versions and redirects everything to them

#include "Globals.h"

#include "ProtocolRecognizer.h"
#include "Protocol125.h"
#include "Protocol132.h"
#include "cClientHandle.h"
#include "cRoot.h"
#include "cWorld.h"
#include "cChatColor.h"





cProtocolRecognizer::cProtocolRecognizer(cClientHandle * a_Client) :
	super(a_Client),
	m_Protocol(NULL),
	m_Buffer(512)
{
}





void cProtocolRecognizer::DataReceived(const char * a_Data, int a_Size)
{
	if (m_Protocol == NULL)
	{
		if (!m_Buffer.Write(a_Data, a_Size))
		{
			m_Client->Kick("Unsupported protocol version");
			return;
		}
		
		if (!TryRecognizeProtocol())
		{
			return;
		}

		// The protocol has just been recognized, dump the whole m_Buffer contents into it for parsing:
		AString Dump;
		m_Buffer.ResetRead();
		m_Buffer.ReadAll(Dump);
		m_Protocol->DataReceived(Dump.data(), Dump.size());
	}
	else
	{
		m_Protocol->DataReceived(a_Data, a_Size);
	}
}





void cProtocolRecognizer::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType);
}





void cProtocolRecognizer::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendBlockChange(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cProtocolRecognizer::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendBlockChanges(a_ChunkX, a_ChunkZ, a_Changes);
}





void cProtocolRecognizer::SendChat(const AString & a_Message)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendChat(a_Message);
}





void cProtocolRecognizer::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendChunkData(a_ChunkX, a_ChunkZ, a_Serializer);
}





void cProtocolRecognizer::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendCollectPickup(a_Pickup, a_Player);
}





void cProtocolRecognizer::SendDestroyEntity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendDestroyEntity(a_Entity);
}





void cProtocolRecognizer::SendDisconnect(const AString & a_Reason)
{
	if (m_Protocol != NULL)
	{
		m_Protocol->SendDisconnect(a_Reason);
	}
	else
	{
		// This is used when the client sends a server-ping, respond with the default packet:
		WriteByte  ((char)0xff);  // PACKET_DISCONNECT
		WriteString(a_Reason);
	}
}





void cProtocolRecognizer::SendEntHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntHeadLook(a_Entity);
}





void cProtocolRecognizer::SendEntLook(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntLook(a_Entity);
}





void cProtocolRecognizer::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
}





void cProtocolRecognizer::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityStatus(a_Entity, a_Status);
}





void cProtocolRecognizer::SendGameMode(eGameMode a_GameMode)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendGameMode(a_GameMode);
}





void cProtocolRecognizer::SendHealth(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendHealth();
}





void cProtocolRecognizer::SendInventoryProgress(char a_WindowID, short a_Progressbar, short a_Value)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendInventoryProgress(a_WindowID, a_Progressbar, a_Value);
}





void cProtocolRecognizer::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendInventorySlot(a_WindowID, a_SlotNum, a_Item);
}





void cProtocolRecognizer::SendKeepAlive(int a_PingID)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendKeepAlive(a_PingID);
}





void cProtocolRecognizer::SendLogin(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendLogin(a_Player);
}





void cProtocolRecognizer::SendMetadata(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendMetadata(a_Entity);
}





void cProtocolRecognizer::SendPickupSpawn(const cPickup & a_Pickup)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPickupSpawn(a_Pickup);
}





void cProtocolRecognizer::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerAnimation(a_Player, a_Animation);
}





void cProtocolRecognizer::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerListItem(a_Player, a_IsOnline);
}





void cProtocolRecognizer::SendPlayerMoveLook(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerMoveLook();
}





void cProtocolRecognizer::SendPlayerPosition(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerPosition();
}





void cProtocolRecognizer::SendPlayerSpawn(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerSpawn(a_Player);
}





void cProtocolRecognizer::SendEntRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntRelMove(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cProtocolRecognizer::SendEntRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cProtocolRecognizer::SendRespawn(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendRespawn();
}





void cProtocolRecognizer::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSpawnMob(a_Mob);
}





void cProtocolRecognizer::SendTeleportEntity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendTeleportEntity(a_Entity);
}





void cProtocolRecognizer::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendTimeUpdate(Int64 a_WorldTime)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendTimeUpdate(a_WorldTime);
}





void cProtocolRecognizer::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUnloadChunk(a_ChunkX, a_ChunkZ);
}





void cProtocolRecognizer::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUpdateSign(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cProtocolRecognizer::SendWeather(eWeather a_Weather)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWeather(a_Weather);
}





void cProtocolRecognizer::SendWholeInventory(const cInventory & a_Inventory)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWholeInventory(a_Inventory);
}





void cProtocolRecognizer::SendWholeInventory(const cWindow & a_Window)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWholeInventory(a_Window);
}





void cProtocolRecognizer::SendWindowClose(char a_WindowID)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWindowClose(a_WindowID);
}





void cProtocolRecognizer::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWindowOpen(a_WindowID, a_WindowType, a_WindowTitle, a_NumSlots);
}





void cProtocolRecognizer::SendData(const char * a_Data, int a_Size)
{
	// This is used only when handling the server ping
	m_Client->SendData(a_Data, a_Size);
}





bool cProtocolRecognizer::TryRecognizeProtocol(void)
{
	// The first packet should be a Handshake, 0x02:
	unsigned char PacketType;
	if (!m_Buffer.ReadByte(PacketType))
	{
		return false;
	}
	switch (PacketType)
	{
		case 0x02: break;  // Handshake, continue recognizing
		case 0xfe: HandleServerPing(); return false;
		default: return false;
	}
	
	// 1.3.2 starts with 0x02 0x39 <name-length-short>
	// 1.2.5 starts with 0x02 <name-length-short> and name is expected to less than 0x3900 long :)
	char ch;
	if (!m_Buffer.ReadChar(ch))
	{
		return false;
	}
	if (ch == 39)
	{
		m_Protocol = new cProtocol132(m_Client);
		return true;
	}
	m_Protocol = new cProtocol125(m_Client);
	return true;
}





void cProtocolRecognizer::HandleServerPing(void)
{
	AString Reply;
	Printf(Reply, "%s%s%i%s%i", 
		cRoot::Get()->GetDefaultWorld()->GetDescription().c_str(), 
		cChatColor::Delimiter.c_str(), 
		cRoot::Get()->GetDefaultWorld()->GetNumPlayers(),
		cChatColor::Delimiter.c_str(), 
		cRoot::Get()->GetDefaultWorld()->GetMaxPlayers()
	);
	m_Client->Kick(Reply.c_str());
}




